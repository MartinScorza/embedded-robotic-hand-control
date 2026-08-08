# UTC hand firmware — publication status

The original academic project contains MSP432 firmware implementing:

- UART command parsing from a Python PC interface;
- SPI communication with a dedicated UTC hand-interface board;
- reads of position/status/board identification;
- PI-based position control;
- predefined `OPEN`, `MID`, `CLOSE` and `DEMO` movements.

The reviewed project report references an internal UTC technical document for the interface board. To avoid publishing an internal register map or university-specific low-level protocol before permission/provenance is confirmed, the board-specific UTC firmware is intentionally not included in this public-safe tree.

This directory can be populated later if redistribution is authorized.
