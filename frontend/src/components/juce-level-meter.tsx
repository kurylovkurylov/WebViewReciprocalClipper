import { FC, useEffect, useState } from "react";

// const rmsRecieverId = window.__JUCE__.backend.addEventListener(
//   "rmsLevels",
//   (e) => console.log(e)
// );

interface JuceLevelMeterProps {
  eventId: string;
  title: string;
}

const minLevel = -100;
const maxLevel = 6;

export const JuceLevelMeter: FC<JuceLevelMeterProps> = ({ eventId, title }) => {
  const [level, setLevel] = useState(0);

  useEffect(() => {
    const rmsEventHandler = (e) => {
      // Update state with the event data
      setLevel(Number(e));
    };

    // Attach the event listener to the RMS receiver
    window.__JUCE__.backend.addEventListener(eventId, rmsEventHandler);

    // Cleanup the event listener on component unmount
    return () => {
      window.__JUCE__.backend.removeEventListener(eventId, rmsEventHandler);
    };
  }, [eventId]);

  const scaleLevel = () => {
    return ((level - minLevel) / (maxLevel - minLevel)) * 100;
  };

  return (
    <div style={{ display: "flex", flexDirection: "column" }}>
      <span style={{ fontSize: "12px" }}>{title}</span>
      <div style={{ background: "black", height: "20px", width: "100%" }}>
        <div
          style={{
            height: "100%",
            width: `${scaleLevel()}%`,
            background: "red",
          }}
        />
      </div>
      {/* <span>{level.toFixed(2)}</span> */}
    </div>
  );
};
