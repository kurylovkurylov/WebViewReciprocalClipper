import * as Juce from "./JUCE";
import { JuceSlider } from "./components/juce-slider";
import { JuceLevelMeter } from "./components/juce-level-meter";
import { CircularSlider } from "./components/circular-slider";
import { TestSlider } from "./test-slider";
import { useState } from "react";

// const rmsRecieverId = window.__JUCE__.backend.addEventListener(
//   "rmsLevelIn",
//   (e) => console.log(e)
// );

export const App = () => {
  const [value, setValue] = useState(0);
  return (
    <div style={{ padding: "20px" }}>
      <TestSlider value={value} onChange={(v: number) => setValue(v)} />
      {/* <CircularSlider /> */}
      {/* <h3
        style={{
          textAlign: "center",
          marginBottom: "20px",
          border: "1px solid black",
        }}
      >
        SPEKADSP
      </h3>
      <div>
        <JuceSlider id="driveSlider" />
        <JuceSlider id="biasSlider" />
        <JuceSlider id="outSlider" />
        <JuceSlider id="mixSlider" />
      </div>
      <div>
        <JuceLevelMeter eventId="rmsLevelIn" title="RMS In: " />
        <JuceLevelMeter eventId="rmsLevelOut" title="RMS Out: " />
      </div> */}
    </div>
  );
};
