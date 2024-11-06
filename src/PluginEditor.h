#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                              private Timer {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
  ~AudioPluginAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

  void timerCallback() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor &processorRef;

  WebSliderRelay driveSliderRelay{"driveSlider"};
  WebSliderRelay biasSliderRelay{"biasSlider"};
  WebSliderRelay outSliderRelay{"outSlider"};
  WebSliderRelay mixSliderRelay{"mixSlider"};

  WebBrowserComponent webView{WebBrowserComponent::Options{}
                                  .withNativeIntegrationEnabled()
                                  .withOptionsFrom(driveSliderRelay)
                                  .withOptionsFrom(biasSliderRelay)
                                  .withOptionsFrom(outSliderRelay)
                                  .withOptionsFrom(mixSliderRelay)};

  WebSliderParameterAttachment driveAttachment;
  WebSliderParameterAttachment biasAttachment;
  WebSliderParameterAttachment outAttachment;
  WebSliderParameterAttachment mixAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
