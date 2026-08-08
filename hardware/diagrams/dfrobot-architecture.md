# DFRobot hand architecture

This diagram is an original portfolio reconstruction from the reviewed firmware. It does not reproduce vendor or internal UTC artwork.

```mermaid
flowchart LR
    J[Joystick] -->|ADC14 A15 / A9| MCU[MSP432P401R]
    FSR[5 x FSR sensors] -->|ADC14 A16–A20| MCU
    FMA[Honeywell FMA sensor] -->|SPI eUSCI_B3| MCU
    BTN[Buttons] -->|GPIO interrupts| MCU

    MCU -->|Timer_A1: 4 PWM channels| SERVOS[5 servomotors]
    MCU -->|Timer_A3: 1 PWM channel| SERVOS
    MCU -->|SPI eUSCI_B0| LCD[128 x 128 LCD]
    MCU -->|Timer_A0 PWM| BUZ[Buzzer]
```

## Control path

The MSP432 directly generates the five servo commands. The reviewed firmware uses a 20 ms PWM period (50 Hz) and constrains command pulse widths to approximately 1000–1600 µs.

The joystick and FSR inputs are prototype-specific mappings to servo commands. The FMA sensor is read digitally over SPI. The firmware does not use measured finger-position feedback, so this architecture is described as open-loop servo commanding rather than closed-loop finger-position control.
