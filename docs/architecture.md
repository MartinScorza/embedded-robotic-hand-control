# Architecture notes

## DFRobot hand

The DFRobot path combines direct actuator control with local sensor acquisition. Joystick and five FSR signals are acquired through ADC14, while the digital FMA sensor is read over a separate SPI peripheral. The MSP432 generates five hardware PWM outputs for the servomotors and also drives the project LCD/buzzer interface.

The reviewed firmware does not use measured finger-position feedback for this hand, so the servo command path should be described as open-loop position commanding rather than closed-loop finger-position control.

## UTC hand

The UTC hand separates high-level control from motor driving:

1. a Python/Tkinter application sends text commands over UART at 9600 baud;
2. the MSP432 parses commands;
3. position/status information is exchanged with a dedicated interface board over SPI;
4. the MSP432 computes PI position-control commands;
5. the interface board performs low-level actuator driving.

The firmware contains predefined `OPEN`, `MID` and `CLOSE` references and a `DEMO` sequence.

The full board-specific protocol is not reproduced here while its redistribution status is under review.
