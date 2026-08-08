# Known limitations and audit findings

This file records limitations found during the portfolio audit. It is intentionally explicit so the repository does not overstate the academic prototype.

## DFRobot firmware

- A selector is read before the GPIO initialization sequence is completed.
- The reviewed selector initialization configures pull resistors on P3 while the later selector read uses P7, indicating a probable port mismatch.
- Some formatted-output strings in the project code appear malformed.
- Joystick/FSR scaling lacks explicit saturation before conversion to an unsigned result, so values outside the calibration range may behave unexpectedly.
- Progressive movements can run blocking delays inside GPIO interrupt handlers.
- No explicit button debounce mechanism was identified.
- FMA acquisition contains blocking waits without a robust transfer timeout.

## UTC architecture

From the reviewed original firmware/audit:

- SysTick is configured for nominal 200 Hz scheduling, but the effective full control-loop frequency was not measured.
- Multiple low-level SPI operations are blocking.
- The SPI wait path does not provide a complete real transfer timeout.
- Some error/alarm handling is defined but not fully integrated in the reviewed flow.
- `OPEN`, `MID`, `CLOSE` and `DEMO` are blocking operations.
- `STOP` therefore must not be described as an emergency stop.
- The original firmware contains a case mismatch between an include spelling (`UART_PC.h`) and the actual file (`uart_pc.h`), which can break builds on case-sensitive filesystems.

No functional source changes have been made in this public-safe reconstruction. These findings should be addressed on dedicated branches with hardware re-validation where appropriate.
