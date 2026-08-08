# Validation evidence

The final academic report/presentation describe a practical hardware demonstration.

## DFRobot platform

Reported/demonstrated behavior includes:

- five PWM servo outputs;
- joystick-based movement;
- FSR-based movement;
- FMA-based movement;
- progressive movements;
- LCD/button/buzzer integration.

## UTC platform

Reported/demonstrated behavior includes:

- Python-to-MSP432 UART communication;
- MSP432-to-board SPI communication;
- board identification using expected value `0x2024`;
- position/status reads;
- predefined `OPEN`, `MID`, `CLOSE` poses;
- `DEMO` sequence;
- PI position control.

## What is not supported by preserved evidence

No preserved benchmark was found for:

- positioning accuracy;
- repeatability;
- overshoot;
- settling time;
- communication error rate;
- measured end-to-end control-loop rate;
- long-duration reliability.

These metrics should not be claimed without new measurements.
