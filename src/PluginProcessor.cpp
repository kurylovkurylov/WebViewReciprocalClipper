#ifdef __INTELLISENSE__
#define JucePlugin_Name "PluginName"
#endif

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor(
    AudioProcessorValueTreeState::ParameterLayout layout)
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      parameters(layout), state(*this, nullptr, "STATE", std::move(layout)) {

  chain.get<static_cast<int>(ChainPosition::WaveShaper)>().functionToUse =
      [this](float x) {
        if (x == 0.f)
          return 0.f;

        static constexpr auto func = [](float v) {
          return v / (1.f + std::abs(v));
        };

        static constexpr float minBias = 1.f;
        static constexpr float maxBias = 4.f;

        float bias =
            minBias + (parameters.bias.get() * .01f) * (maxBias - minBias);

        return x > 0.f ? func(x * bias) / bias : func(x / bias) * bias;
      };
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String &newName) {
  juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  juce::dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
  spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

  constexpr float sliderRampTime = .005f;

  chain.get<static_cast<int>(ChainPosition::DrivePreGain)>()
      .setRampDurationSeconds(sliderRampTime);
  chain.get<static_cast<int>(ChainPosition::DrivePostGain)>()
      .setRampDurationSeconds(sliderRampTime);
  chain.get<static_cast<int>(ChainPosition::OutGain)>().setRampDurationSeconds(
      sliderRampTime);

  auto dcCoeff = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(
      getSampleRate(), 1.0f);

  dcFilter.state = *dcCoeff;

  chain.prepare(spec);
  dryWetMixer.prepare(spec);
  dcFilter.prepare(spec);

  rmsIn.reset(sampleRate, 0.4);
  rmsIn.setCurrentAndTargetValue(-100.f);
  rmsOut.reset(sampleRate, 0.4);
  rmsOut.setCurrentAndTargetValue(-100.f);
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  juce::dsp::AudioBlock<float> block(buffer);
  juce::dsp::ProcessContextReplacing<float> context(block);
  // ======================================================
  float rmsInLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
  float rmsInRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
  // rmsIn = Decibels::gainToDecibels((rmsInLeft + rmsInRight) * .5f);

  rmsIn.skip(buffer.getNumSamples());

  const auto rmsInValue =
      Decibels::gainToDecibels((rmsInLeft + rmsInRight) * .5f);
  rmsIn.setTargetValue(rmsInValue);
  // if (rmsInValue < rmsIn.getCurrentValue()) {
  // } else {
  //   rmsIn.setCurrentAndTargetValue(rmsInValue);
  // }

  chain.get<static_cast<int>(ChainPosition::DrivePreGain)>().setGainDecibels(
      parameters.drive.get());
  chain.get<static_cast<int>(ChainPosition::DrivePostGain)>().setGainDecibels(
      -parameters.drive.get());
  chain.get<static_cast<int>(ChainPosition::OutGain)>().setGainDecibels(
      parameters.out.get());

  dryWetMixer.setWetMixProportion(parameters.mix.get() * .01f);

  dryWetMixer.pushDrySamples(block);
  chain.process(context);
  dryWetMixer.mixWetSamples(block);

  dcFilter.process(context);

  float rmsOutLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
  float rmsOutRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
  // rmsOut = Decibels::gainToDecibels((rmsOutLeft + rmsOutRight) * .5f);

  rmsOut.skip(buffer.getNumSamples());

  const auto rmsOutValue =
      Decibels::gainToDecibels((rmsOutLeft + rmsOutRight) * .5f);
  rmsOut.setTargetValue(rmsOutValue);
  // if (rmsOutValue < rmsOut.getCurrentValue()) {
  // } else {
  //   rmsOut.setCurrentAndTargetValue(rmsOutValue);
  // }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  juce::MemoryOutputStream mos(destData, true);
  state.state.writeToStream(mos);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data,
                                                    int sizeInBytes) {
  auto tree =
      juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));

  if (tree.isValid()) {
    state.replaceState(tree);
  }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new AudioPluginAudioProcessor({});
}
