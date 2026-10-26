# Working Principle

![Flowchart](flowchart.png)

## Step by step

1. **Idle**: belt is stopped, servo is at neutral (90°).
2. **Detect**: an object is placed at the start of the belt. The IR sensor output goes LOW.
3. **Move**: the belt starts (PWM speed `BELT_SPEED`) and runs for `TRAVEL_TO_SENSOR_MS` so the object reaches the area under the TCS3200.
4. **Stop and read**: the belt stops. The TCS3200 measures the red, green and blue channels, 5 samples each, and the firmware takes the average.
5. **Classify**: the TCS3200 gives a pulse width, so a *smaller* value means *more* of that colour. A colour wins only if it is clearly smaller than the other two (`DOMINANCE_PERCENT`). If all channels are too large, or no channel clearly wins, the result is UNKNOWN.
6. **Divert**: the servo moves to the angle of that colour.

   | Colour | Servo angle |
   |---|---|
   | Red | 30° |
   | Green | 90° |
   | Blue | 150° |
   | Unknown | 90° (neutral, object passes straight) |

7. **Deliver**: the belt runs again for `SORT_RUN_MS` so the object leaves the belt into the correct bin.
8. **Reset**: belt stops, servo returns to neutral, system waits for the next object.

## Differences from the earlier banana-quality version

| Earlier version | Colour sorting version |
|---|---|
| Good / bad banana (2 classes) | Red / Green / Blue / Unknown |
| Fixed thresholds in the condition | Dominant-colour logic with tunable percentage |
| Belt ran 10 s on every IR trigger | Stop-and-read cycle with separate travel and sort times |
| Single reading | Averaged readings (5 samples) |
| No calibration help | Serial calibration mode |
