# Known limitations and audit findings

This file records engineering limitations found during the portfolio audit. It distinguishes findings that were corrected during repository hardening from limitations that remain in the current source.

## DFRobot firmware

### Resolved during portfolio hardening

The following deterministic defects were corrected and clean-build validated on 2026-08-08:

- sensor-selector GPIO is now initialized before `Read_ActiveSensor()` is called;
- selector direction/pull resistors are now configured on P7.1/P7.2, matching the existing read path;
- three malformed LCD mode/status format strings were corrected;
- joystick inputs are clamped to the configured calibration interval before servo mapping;
- FSR inputs are clamped to the configured calibration interval before servo mapping.

These fixes were compiled and linked successfully using SimpleLink MSP432P4 SDK 3.40.01.02 and TI ARM Compiler 20.2.7.LTS in a clean environment. Physical re-validation of the corrected portfolio revision has not been claimed.

### Remaining limitations

- Progressive movements can run blocking delays inside GPIO interrupt handlers. The ISR can therefore remain active for an extended period.
- No explicit button debounce mechanism was identified.
- FMA acquisition contains blocking waits without a robust transfer timeout.
- The DFRobot architecture commands servo pulse widths without measured finger-position feedback.
- Joystick, FSR and FMA mappings are prototype-specific calibration mappings rather than general calibrated physical-force models.

## UTC architecture

From the reviewed original firmware/audit:

- SysTick is configured for nominal 200 Hz scheduling, but the effective full control-loop frequency was not measured.
- Multiple low-level SPI operations are blocking.
- The SPI wait path does not provide a complete real transfer timeout.
- Some error/alarm handling is defined but not fully integrated in the reviewed flow.
- `OPEN`, `MID`, `CLOSE` and `DEMO` are blocking operations.
- `STOP` therefore must not be described as an emergency stop.
- The original UTC firmware contains a case mismatch between an include spelling (`UART_PC.h`) and the actual file (`uart_pc.h`), which can break builds on case-sensitive filesystems.

The low-level UTC board-specific source is intentionally not included in the portfolio repository, so these UTC findings are documented from the audited original project rather than presented as corrected public-source behavior.

## Validation boundary

The current DFRobot source is **clean-build validated**. The original project documentation supports physical operation of both robotic-hand setups, but this portfolio reconstruction has not added new physical hardware measurements. See [`reproducibility-status.md`](reproducibility-status.md) for details.
