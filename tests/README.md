# Test and reproducibility plan

The original project was demonstrated on physical hardware but did not include a reusable automated firmware test suite.

## Completed during portfolio reconstruction

- [x] Reproduced a clean DFRobot firmware build in a fresh environment.
- [x] Used the documented legacy TI SDK/compiler versions.
- [x] Compiled and linked without user-specific absolute paths.
- [x] Produced a non-empty firmware image.
- [x] Recorded that no firmware compile/link warnings were observed.
- [x] Added automated Python-interface smoke checks.
- [x] Added an automated public-release hygiene guard.
- [x] Corrected and clean-build validated the deterministic selector/formatting/input-clamping findings.

## Remaining physical / deeper validation

If the original hardware is available:

1. verify all five DFRobot PWM outputs with an oscilloscope;
2. verify representative 50 Hz / 1000–1600 µs servo commands;
3. test selector, joystick, FSR and FMA boundary behavior on the physical setup;
4. confirm LCD mode/status display after the source corrections;
5. measure the effective UTC control-loop frequency if the board is available.

Independent of hardware availability, future engineering work could also:

- test UART command parsing with a host-side/mock harness;
- exercise failure handling for unavailable SPI devices;
- refactor blocking interrupt/control paths only with appropriate regression validation.

All future results should continue to distinguish newly reproduced evidence from behavior reported in the original academic demonstration.
