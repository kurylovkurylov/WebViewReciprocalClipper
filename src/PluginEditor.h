#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final
    : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
  ~AudioPluginAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor &processorRef;

  WebSliderRelay driveSliderRelay{"driveSlider"};
  WebSliderRelay biasSliderRelay{"biasSlider"};
  WebSliderRelay outSliderRelay{"outSlider"};

  WebBrowserComponent webView{WebBrowserComponent::Options{}
                                  .withOptionsFrom(driveSliderRelay)
                                  .withOptionsFrom(biasSliderRelay)
                                  .withOptionsFrom(outSliderRelay)};

  WebSliderParameterAttachment driveAttachment;
  WebSliderParameterAttachment biasAttachment;
  WebSliderParameterAttachment outAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
