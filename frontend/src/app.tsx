import { JuceSlider } from "./components/juce-slider";

export const App = () => {
  return (
    <div style={{ padding: "20px" }}>
      <h3 style={{ textAlign: "center", paddingBottom: "20px" }}>SPEKA</h3>
      <JuceSlider id="driveSlider" />
      <JuceSlider id="biasSlider" />
      <JuceSlider id="outSlider" />
      <JuceSlider id="mixSlider" />
    </div>
  );
};
