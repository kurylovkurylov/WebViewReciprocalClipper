import { FC, useCallback, useRef, useState } from "react";

const SENS = 0.005;
const MIN_VALUE = 0;
const MAX_VALUE = 1;
const MIN_ROTATION = -135;
const MAX_ROTATION = 135;

const clamp = (value: number, min: number, max: number) => {
  return Math.min(Math.max(value, min), max);
};

const getRotation = (
  value: number,
  minRotation: number,
  maxRotation: number
) => {
  const min = -135;
  const max = 135;
  return clamp(minRotation + (maxRotation - minRotation) * value, min, max);
};

interface TestSliderProps {
  value: number;
  onChange: (value: number) => void;
  min?: number;
  max?: number;
  sens?: number;
  minRotation?: number;
  maxRotation?: number;
}

export const TestSlider: FC<TestSliderProps> = (props) => {
  const {
    value,
    onChange,
    min = MIN_VALUE,
    max = MAX_VALUE,
    sens = SENS,
    minRotation = MIN_ROTATION,
    maxRotation = MAX_ROTATION,
  } = props;

  // const [isDragging, setIsDragging] = useState(false);
  const lastPosition = useRef<{ x: number; y: number } | null>(null);

  const handleMouseDown = (e: React.MouseEvent) => {
    lastPosition.current = { x: e.clientX, y: e.clientY };
    // setIsDragging(true);

    console.log("Mouse down");

    window.addEventListener("mousemove", handleMouseMove);
    window.addEventListener("mouseup", handleMouseUp);
  };

  const handleMouseUp = () => {
    // setIsDragging(false);
    lastPosition.current = null;

    console.log("Mouse up");

    window.removeEventListener("mousemove", handleMouseMove);
    window.removeEventListener("mouseup", handleMouseUp);
  };

  const handleMouseMove = useCallback(
    (e: MouseEvent) => {
      console.log("Mouse move");
      console.log("Last position: ", lastPosition.current);
      // if (!isDragging || !lastPosition.current) return;
      if (!lastPosition.current) return;

      const { x: lastX, y: lastY } = lastPosition.current;

      const deltaX = e.clientX - lastX;
      const deltaY = lastY - e.clientY;

      const delta = deltaX + deltaY;

      console.log("Delta: ", delta);

      onChange(clamp(value + delta * sens, min, max));

      lastPosition.current = { x: e.clientX, y: e.clientY };
    },
    [value, onChange, min, max, sens]
  );

  return (
    <div
      style={{
        display: "flex",
        flexDirection: "column",
      }}
    >
      <div>Value: {value}</div>
      <div
        style={{
          width: 400,
          height: 400,
          border: "1px solid black",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
        }}
      >
        <div
          onMouseDown={handleMouseDown}
          style={{
            position: "relative",
            width: 250,
            height: 250,
            background: "gray",
            borderRadius: "50%",
            transform: `rotate(${getRotation(
              value,
              minRotation,
              maxRotation
            )}deg)`,
          }}
        >
          <div
            style={{
              width: 10,
              height: 10,
              background: "white",
              margin: "10px auto",
              borderRadius: "50%",
            }}
          ></div>
        </div>
      </div>
    </div>
  );
};
