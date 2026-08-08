# Hardware diagrams

This directory contains original, public-safe portfolio diagrams reconstructed from the reviewed project code and documentation.

## Available diagrams

1. [`dfrobot-architecture.md`](dfrobot-architecture.md) — joystick / FSR / FMA inputs → MSP432P401R → five direct PWM servo outputs, LCD and buzzer.
2. [`utc-architecture.md`](utc-architecture.md) — Python GUI → UART → MSP432P401R / PI control → SPI → UTC interface board → four actuators.
3. [`pi-control-loop.md`](pi-control-loop.md) — public-safe representation of the UTC-hand PI position-control loop.

The diagrams use Mermaid so they render directly on GitHub while remaining reviewable and version-controlled as text.

## Still pending

A complete DFRobot wiring/power schematic should only be added after the final external servo supply, sensor conditioning network and selector-switch wiring are confirmed from the physical setup or reliable original records.

Vendor artwork and internal UTC technical diagrams are intentionally not copied into this repository.
