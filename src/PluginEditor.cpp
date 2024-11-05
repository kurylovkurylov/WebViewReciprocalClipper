#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      driveAttachment(*processorRef.state.getParameter(ID::drive.getParamID()),
                      driveSliderRelay),
      biasAttachment(*processorRef.state.getParameter(ID::bias.getParamID()),
                     biasSliderRelay),
      outAttachment(*processorRef.state.getParameter(ID::out.getParamID()),
                    outSliderRelay) {
  juce::ignoreUnused(processorRef);
  addAndMakeVisible(webView);
  webView.goToURL("https://www.juce.com");

  setSize(800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized() {
  webView.setBounds(getLocalBounds());
}
