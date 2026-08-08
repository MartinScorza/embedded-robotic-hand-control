# Hardware requirements

## Common embedded platform

- MSP-EXP432P401R LaunchPad / MSP432P401R target
- USB/XDS110 debug/serial connection

## DFRobot platform

Verified from code/project documentation:

- five servo channels;
- BOOSTXL-EDUMKII peripherals used by the firmware;
- joystick;
- five FSR inputs;
- Honeywell FMA-series digital force sensor;
- LCD, buttons and buzzer.

Still to confirm before publishing a complete replication guide:

- exact robotic-hand commercial model;
- exact FSR part/reference;
- exact FMA part number and pressure/force range;
- external servo supply voltage/current;
- final sensor conditioning resistors and wiring;
- final selector-switch wiring.

## UTC platform

The documented architecture uses:

- MSP432P401R;
- dedicated UTC robotic-hand interface board;
- four-actuator robotic hand;
- PC/XDS110 UART connection;
- SPI connection between MSP432 and the interface board.

Board-specific electrical/pinout documentation is intentionally withheld pending redistribution review.
