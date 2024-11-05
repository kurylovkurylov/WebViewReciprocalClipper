#pragma once

#include "Parameters.h"
#include <JuceHeader.h>

using Gain = dsp::Gain<float>;
using WaveShaper = dsp::WaveShaper<float, std::function<float(float)>>;
using Filter = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>,
                                        dsp::IIR::Coefficients<float>>;
using Chain = dsp::ProcessorChain<Gain, WaveShaper, Filter, Gain, Gain>;
using DryWet = dsp::DryWetMixer<float>;

enum class ChainPosition {
  DrivePreGain,
  WaveShaper,
  DCFilter,
  DrivePostGain,
  OutGain
};

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor {
public:
  //==============================================================================
  AudioPluginAudioProcessor(
      AudioProcessorValueTreeState::ParameterLayout layout);
  ~AudioPluginAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  using AudioProcessor::processBlock;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  Parameters parameters;
  AudioProcessorValueTreeState state;

private:
  Chain chain;
  DryWet dryWetMixer;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
