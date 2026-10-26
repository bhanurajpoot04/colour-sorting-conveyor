# Calibration Guide

Colour sensor readings change with room light, sensor height and object material. Calibrate once at the place where you will demonstrate the project.

## Steps

1. Upload the firmware and open **Serial Monitor** at **9600 baud**.
2. Send the character `c`. Calibration mode turns on and the sensor prints `R, G, B` every 0.5 s.
3. Place an object of each colour under the sensor and note the values.

   | Object | R | G | B | Result printed |
   |---|---|---|---|---|
   | Red | | | | |
   | Green | | | | |
   | Blue | | | | |
   | Empty belt | | | | |

4. Remember: **lower number = stronger colour**. A red object should give the lowest R value.
5. Tune in the sketch:
   - `NO_OBJECT_THRESHOLD`: set slightly below the values you see on an empty belt.
   - `DOMINANCE_PERCENT`: if colours are misdetected as UNKNOWN, increase it (e.g. 90). If wrong colours are accepted, decrease it (e.g. 80).
6. Send `c` again to leave calibration mode.

## Mechanical tuning

| Setting | What to adjust |
|---|---|
| `BELT_SPEED` | Slower belt gives more stable readings (0-255) |
| `TRAVEL_TO_SENSOR_MS` | Object should stop exactly under the TCS3200 |
| `SORT_RUN_MS` | Object should fully leave the belt into the bin |
| `ANGLE_RED / GREEN / BLUE` | Match the diverter flap direction to each bin |

## Tips for stable readings

- Keep the sensor 1-3 cm above the object and inside the shaded gantry.
- Keep the TCS3200 white LEDs on.
- Use matte objects; shiny surfaces reflect light and confuse the sensor.
- Avoid direct sunlight or strong lights near the belt.

## Troubleshooting

| Problem | Likely cause / fix |
|---|---|
| Belt does not move | ENA jumper still fitted, no 12V, or IN1/IN2 swapped |
| Belt runs in wrong direction | Swap OUT1 and OUT2 wires or swap the HIGH/LOW in `beltOn()` |
| Always UNKNOWN | Thresholds too strict, or sensor too far, or LEDs off |
| Servo jitters / Arduino resets | Use separate 5V supply for servo, common GND |
| IR never triggers | Adjust the IR potentiometer; check that it is active LOW |
