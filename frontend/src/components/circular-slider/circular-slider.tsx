import { useRef, useState } from "react";
import styles from "./styles.module.css";

export const CircularSlider = () => {
  const [value, setValue] = useState(0);
  //   const [isDragging, setIsDragging] = useState(false);
  const rotatorRef = useRef<HTMLDivElement>(null);

  const handleMouseDown = () => {
    console.log("Mouse down");
    // setIsDragging(true);

    window.addEventListener("mousemove", handleMouseMove);
    window.addEventListener("mouseup", handleMouseUp);
  };

  const handleMouseUp = () => {
    console.log("Mouse up");
    // setIsDragging(false);

    window.removeEventListener("mousemove", handleMouseMove);
    window.removeEventListener("mouseup", handleMouseUp);
  };

  const handleMouseMove = (e: MouseEvent) => {
    if (!rotatorRef.current) return;

    const rect = rotatorRef.current.getBoundingClientRect();
    const centerX = rect.left + rect.width / 2;
    const centerY = rect.top + rect.height / 2;

    const angleRad = Math.atan2(e.clientY - centerY, e.clientX - centerX);
    const angleDeg = (angleRad * 180) / Math.PI;
    console.log("angle deg: ", angleDeg);

    const rotation = (angleDeg + 180) % 360;

    console.log("angle deg: ", angleDeg);
    console.log("rotation: ", rotation);

    setValue(rotation);
  };

  return (
    // <div className={styles.Container}>
    //   <div className={styles.Slider}>
    //     <div className={styles.Knob} onMouseDown={handleMouseDown}>
    //       <div className={styles.Rotator}>
    //         <div className={styles.Text}>{value}</div>
    //       </div>
    //       <div className={styles.Pointer}>{"<"}</div>
    //     </div>
    //     <svg width="100%" height="100%">
    //       <circle className={styles.circle1} cx="50%" cy="50%" r="40%" />
    //       <circle className={styles.circle2} cx="50%" cy="50%" r="40%" />
    //     </svg>
    //   </div>
    // </div>
    //   return (
    <div className={styles.Container}>
      <div className={styles.Slider}>
        <div className={styles.Knob} onMouseDown={handleMouseDown}>
          <div
            className={styles.Rotator}
            ref={rotatorRef}
            style={{
              transform: `rotate(${value}deg)`,
            }}
          >
            <div className={styles.Pointer} />
          </div>
        </div>
      </div>
    </div>
  );
};

// export const CircularSlider = () => {
//   return (
//     <div className={styles.Container}>
//       <div className={styles.Slider}>
//         <div className={styles.Knob}>
//           <div className={styles.Rotator}>
//             <div className={styles.Pointer} />
//           </div>
//         </div>
//       </div>
//     </div>
//   );
// };
