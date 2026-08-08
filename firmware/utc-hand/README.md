# UTC hand firmware — portfolio scope

The original academic project contains MSP432 firmware implementing:

- UART command parsing from a Python PC interface;
- SPI communication with a dedicated UTC hand-interface board;
- reads of position, status and board identification;
- PI-based position control;
- predefined `OPEN`, `MID`, `CLOSE` and `DEMO` movements.

The reviewed project report references an internal UTC technical document for the interface board. To avoid publishing an internal register map or university-specific low-level protocol without verified redistribution rights, the board-specific UTC firmware is intentionally **not included** in this portfolio repository.

This directory is therefore documentation-only by design. The public portfolio presents the UTC subsystem at architecture, command and demonstrated-functionality level, which is sufficient to show the embedded-control work without exposing uncertain internal material.

See the [UTC architecture](../../hardware/diagrams/utc-architecture.md), [PI control loop](../../hardware/diagrams/pi-control-loop.md) and [validation evidence](../../docs/validation.md).
