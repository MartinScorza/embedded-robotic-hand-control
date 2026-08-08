# DFRobot hand — firmware pin mapping

This mapping is reconstructed directly from the reviewed firmware. It is a software pin map, not a complete wiring/power schematic.

## Servo PWM

| Servo | MSP432 pin | Timer output |
|---|---|---|
| 1 | P7.7 | TA1.1 |
| 2 | P7.6 | TA1.2 |
| 3 | P7.5 | TA1.3 |
| 4 | P7.4 | TA1.4 |
| 5 | P8.2 | TA3.2 |

## ADC inputs

| Input | MSP432 analog input | ADC memory |
|---|---|---|
| Joystick X | P6.0 / A15 | MEM0 |
| Joystick Y | P4.4 / A9 | MEM1 |
| FSR 1 | P9.1 / A16 | MEM2 |
| FSR 2 | P9.0 / A17 | MEM3 |
| FSR 3 | P8.7 / A18 | MEM4 |
| FSR 4 | P8.6 / A19 | MEM5 |
| FSR 5 | P8.5 / A20 | MEM6 |

## FMA SPI

The reviewed firmware uses eUSCI_B3 for the digital FMA sensor, including P10.1 for clock, P10.3 for receive data and P10.2 as a software-controlled chip-select signal.

## Important

The exact external actuator supply, sensor resistor network, commercial hand model and final selector-switch wiring must be confirmed from the physical setup before a complete wiring guide is published.
