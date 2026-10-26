# Pin Connections

Diagram: [`wiring_diagram.svg`](wiring_diagram.svg) / [`wiring_diagram.png`](wiring_diagram.png)

![Wiring diagram](wiring_diagram.png)

## Arduino Uno pin map

| Arduino Pin | Connected to | Function |
|---|---|---|
| D2 | IR sensor OUT | Object detection (active LOW) |
| D3 (PWM) | L298N ENA | Belt motor speed |
| D4 | TCS3200 S0 | Frequency scaling |
| D5 | TCS3200 S1 | Frequency scaling |
| D6 | TCS3200 S2 | Colour filter select |
| D7 | TCS3200 S3 | Colour filter select |
| D8 | TCS3200 OUT | Colour frequency output |
| D9 | Servo signal | Diverter angle |
| D10 | L298N IN1 | Motor direction |
| D11 | L298N IN2 | Motor direction |
| 5V | IR VCC, TCS3200 VCC, Servo VCC | Sensor and servo power |
| GND | All module GNDs + L298N GND | Common ground |

## Module wise

| Module | Pin | Goes to |
|---|---|---|
| TCS3200 | S0, S1, S2, S3, OUT | D4, D5, D6, D7, D8 |
| TCS3200 | VCC, GND | 5V, GND |
| TCS3200 | LED | 5V (keeps white LEDs on) |
| IR sensor | OUT | D2 |
| IR sensor | VCC, GND | 5V, GND |
| L298N | ENA, IN1, IN2 | D3, D10, D11 |
| L298N | OUT1, OUT2 | DC motor terminals |
| L298N | +12V, GND | 12V supply + / - |
| Servo | Signal (orange) | D9 |
| Servo | VCC (red), GND (brown) | 5V, GND |

## TCS3200 settings used

| S0 | S1 | Output frequency scaling |
|---|---|---|
| HIGH | LOW | 20% (used in this project) |

| S2 | S3 | Filter |
|---|---|---|
| LOW | LOW | Red |
| HIGH | HIGH | Green |
| LOW | HIGH | Blue |

## Notes

- Connect Arduino GND, L298N GND and 12V supply GND together (common ground).
- Remove the ENA jumper on the L298N so PWM speed control works.
- The Servo library disables PWM on D9 and D10 of the Uno, so ENA is on D3.
- If the servo jitters or Arduino resets when the servo moves, power the servo from a separate 5V supply (keep GND common).
