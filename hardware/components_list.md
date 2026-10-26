# Components List (Bill of Materials)

> The list below was prepared from the project photos and the firmware pin usage.
> Please verify the exact model / size of mechanical parts and fill in the **Price** column with your own purchase cost.
> A spreadsheet-friendly copy is available in [`components_list.csv`](components_list.csv).

## Electronics

| # | Component | Qty | Specification | Purpose in project | Price (₹) |
|---|---|---|---|---|---|
| 1 | Arduino Uno R3 | 1 | ATmega328P, 5V logic | Main controller running the sorting firmware | |
| 2 | TCS3200 colour sensor module | 1 | RGB colour-to-frequency converter, with 4 white LEDs | Detects object colour (Red / Green / Blue) | |
| 3 | IR obstacle sensor module | 1 | Active LOW output (e.g. FC-51 type) | Detects an object placed on the belt and starts the conveyor | |
| 4 | L298N motor driver module | 1 | Dual H-bridge, 5-35V | Drives the belt DC motor, direction and speed (PWM) | |
| 5 | DC motor | 1 | 12V, coupled to belt roller shaft | Moves the conveyor belt | |
| 6 | Servo motor | 1 | SG90 / MG90S class, 5V | Rotates the diverter flap to push object to the correct bin | |
| 7 | Power supply | 1 | 12V DC adapter or battery pack (2A recommended) | Powers motor through L298N | |
| 8 | USB cable (Type-B) | 1 | For Arduino Uno | Programming and powering Arduino | |
| 9 | Perfboard / breadboard | 1 | Small | Common 5V / GND distribution and header pins | |
| 10 | Slide / toggle switch | 1 | Optional | Main ON/OFF | |
| 11 | Jumper wires | 1 set | Male-male, male-female | Connections between modules | |
| 12 | Zip ties, tape | - | - | Cable management | |

## Mechanical

| # | Component | Qty | Specification | Purpose in project | Price (₹) |
|---|---|---|---|---|---|
| 13 | Conveyor belt | 1 | Blue PVC / PU flat belt, closed loop | Carries the objects | |
| 14 | Rollers (drive + idler) | 2 | With shaft | Belt drive and tension | |
| 15 | Roller holders / bearing stands | 4 | Black, 3D-printed (as seen in photos) | Support the roller shafts | |
| 16 | Side frame | 2 | White | Belt guide and structure | |
| 17 | Sensor gantry (arch) | 1 | White, spans the belt | Holds the TCS3200 above the belt and shades it from ambient light | |
| 18 | Servo mount + diverter flap | 1 | Clear acrylic | Holds the servo and pushes objects off the belt | |
| 19 | Motor mount | 1 | Wood / MDF block | Aligns motor shaft with drive roller | |
| 20 | Motor-roller coupling | 1 | Shaft coupler | Transfers motor torque to roller | |
| 21 | Base board | 1 | MDF / hard board | Mounts everything | |
| 22 | Sorting bins / trays | 3 | Any small boxes | Collect Red, Green and Blue objects | |
| 23 | Test objects | several | Red, green and blue balls / blocks | Demonstration | |
| 24 | Screws, nuts, bolts | - | M3 | Assembly | |

## Tools used

- Wire stripper / cutter
- Screwdriver
- Soldering iron (perfboard connections)
- Multimeter (continuity and voltage checks)
- 3D printer / laser cutter (for printed and acrylic parts)
