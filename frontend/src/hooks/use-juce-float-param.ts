import { ChangeEvent, useEffect, useState } from "react";
import * as Juce from "../JUCE";

const getFormattedValue = (
  properties: Juce.SliderProperties,
  sliderState: Juce.SliderState
) => {
  // Fix floating-point precision by formatting interval to 6 decimal places
  const interval = Number(properties.interval.toFixed(6));

  const decimalPlaces = interval.toString().split(".")[1]?.length || 0;
  const scaledValue = sliderState.getScaledValue();

  return scaledValue.toFixed(decimalPlaces);
};

export const useJuceFloatParam = (paramId: string) => {
  const sliderState = Juce.getSliderState(paramId);

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

  const formattedValue = getFormattedValue(properties, sliderState);

  return { value, formattedValue, properties, handleChange };
};
