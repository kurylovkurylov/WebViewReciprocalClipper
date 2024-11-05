#pragma once

#include <JuceHeader.h>

namespace ID {
#define PARAMETER_ID(str) static const ParameterID str{#str, 1};

PARAMETER_ID(drive)
PARAMETER_ID(bias)
PARAMETER_ID(out)
PARAMETER_ID(mix)

#undef PARAMETER_ID
} // namespace ID

struct Parameters {
public:
  explicit Parameters(AudioProcessorValueTreeState::ParameterLayout &layout)
      : drive(addToLayout<AudioParameterFloat>(
            layout, ID::drive, "Drive",
            NormalisableRange<float>{-24.0f, 24.0f, 0.1f}, 0.0f,
            AudioParameterFloatAttributes{}.withLabel("dB"))),
        bias(addToLayout<AudioParameterFloat>(
            layout, ID::bias, "Bias", NormalisableRange<float>{0.f, 100.f, .1f},
            0.0f, AudioParameterFloatAttributes{}.withLabel("%"))),
        out(addToLayout<AudioParameterFloat>(
            layout, ID::out, "Out",
            NormalisableRange<float>{-24.0f, 24.0f, 0.1f}, 0.0f,
            AudioParameterFloatAttributes{}.withLabel("dB"))),
        mix(addToLayout<AudioParameterFloat>(
            layout, ID::mix, "Mix", NormalisableRange<float>{0.f, 100.f, .1f},
            100.f, AudioParameterFloatAttributes{}.withLabel("%"))) {}

  AudioParameterFloat &drive;
  AudioParameterFloat &bias;
  AudioParameterFloat &out;
  AudioParameterFloat &mix;

private:
  template <typename Param>
  static void add(AudioProcessorParameterGroup &group,
                  std::unique_ptr<Param> param) {
    group.addChild(std::move(param));
  }

  template <typename Param>
  static void add(AudioProcessorValueTreeState::ParameterLayout &group,
                  std::unique_ptr<Param> param) {
    group.add(std::move(param));
  }

  template <typename Param, typename Group, typename... Ts>
  static Param &addToLayout(Group &layout, Ts &&...ts) {
    auto param = std::make_unique<Param>(std::forward<Ts>(ts)...);
    auto &ref = *param;
    add(layout, std::move(param));
    return ref;
  }
};