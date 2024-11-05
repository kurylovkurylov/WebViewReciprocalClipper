import { ChangeEvent, FC, useEffect, useState } from "react";
// @ts-expect-error no types
import * as Juce from "../JUCE";

interface JuceSliderProps {
  id: string;
  //   title: string;
}

export const JuceSlider: FC<JuceSliderProps> = ({ id }) => {
  const sliderState = Juce.getSliderState(id);

  const [value, setValue] = useState(sliderState.getNormalisedValue());
  const [properties, setProperties] = useState(sliderState.properties);

  const handleChange = (event: ChangeEvent<HTMLInputElement>) => {
    const newValue = parseFloat(event.target.value);

    sliderState.setNormalisedValue(newValue);
    setValue(newValue);
  };

  //   const handleMouseDown = () => {
  //     sliderState.sliderDragStarted();
  //   };

  //   const changeCommitted = (event, newValue) => {
  //     sliderState.setNormalisedValue(newValue);
  //     sliderState.sliderDragEnded();
  //   };

  //   const calculateValue = () => {
  //     return sliderState.getScaledValue();
  //   };

  useEffect(() => {
    const valueListenerId = sliderState.valueChangedEvent.addListener(() => {
      setValue(sliderState.getNormalisedValue());
    });

    const propertiesListenerId = sliderState.propertiesChangedEvent.addListener(
      () => setProperties(sliderState.properties)
    );

    return () => {
      sliderState.valueChangedEvent.removeListener(valueListenerId);
      sliderState.propertiesChangedEvent.removeListener(propertiesListenerId);
    };
  });

  const getFormattedValue = () => {
    // Fix floating-point precision by formatting interval to 6 decimal places
    const interval = Number(properties.interval.toFixed(6));

    const decimalPlaces = interval.toString().split(".")[1]?.length || 0;
    const scaledValue = sliderState.getScaledValue();

    return scaledValue.toFixed(decimalPlaces);
  };

  return (
    <div style={{ display: "flex", flexDirection: "column" }}>
      <label>
        {properties.name}: {getFormattedValue()} {properties.label}
      </label>
      <input
        type="range"
        value={value}
        onChange={handleChange}
        min={0}
        max={1}
        step={1 / (properties.numSteps - 1)}
        // onMouseDown={handleMouseDown}
      />
    </div>
  );
};
