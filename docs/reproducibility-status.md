# Reproducibility and static-validation status

This document records checks performed after the initial portfolio reconstruction. It separates what can be verified without the original hardware from what still requires Code Composer Studio and physical validation.

## Baseline

Portfolio baseline on `main`:

- commit: `c7e5836bb31469e84388d8548e33ba502da9ad89`;
- DFRobot application firmware included without functional changes;
- Python/Tkinter UART interface included;
- UTC board-specific firmware intentionally withheld pending provenance/redistribution review.

## Checks completed without hardware

### Python interface

- `python -m py_compile python-interface/robotic_hand_gui.py`: **PASS**.
- The interface declares `pyserial` in `python-interface/requirements.txt`.
- Full runtime import was not executed in the audit environment because `pyserial` is not installed there; this is an environment dependency, not a syntax failure.

### Repository hygiene

The portfolio tree excludes:

- `Debug/` and `Release/` build directories;
- `.out`, `.obj`, `.map`, `.d` and generated makefile fragments;
- historical CCS launch files;
- archive files;
- the internal UTC technical PDFs;
- UTC branding assets;
- board-specific UTC register-map/interface source.

### DFRobot static findings reconfirmed

The following findings are directly observable in the reviewed source:

1. `Read_ActiveSensor()` is called before `GPIO_init()`, so the selector pins are read before the intended GPIO setup is performed.
2. The selector setup comment and read path use P7.1/P7.2, but `GPIO_init()` configures direction/pull resistors on P3.1/P3.2. This is a probable port mismatch.
3. Three LCD formatting calls use `sprintf(..., "%3u%", ...)`, leaving a trailing `%` conversion marker. They should be treated as invalid format strings.
4. Joystick and FSR mappings calculate directly into unsigned 16-bit return fields without explicitly clamping the sensor input/result to the configured calibration range first.
5. `PORT1_IRQHandler()` executes progressive movement functions containing blocking delays. The ISR can therefore remain active for an extended period.
6. The FMA SPI read path waits indefinitely for `RXIFG`; no robust communication timeout is implemented.

These items are not presented as newly discovered runtime failures. They are static implementation defects/risks that require controlled correction and, for hardware-facing changes, re-validation.

## Checks still blocked

The following cannot be claimed as reproduced from the current environment:

- clean MSP432 firmware build using the original CCS/SDK toolchain;
- successful flashing through XDS110;
- five-channel PWM validation on an oscilloscope;
- final selector-switch wiring and behavior;
- ADC boundary behavior on physical joystick/FSR inputs;
- FMA communication and timeout behavior on hardware;
- effective end-to-end UTC PI control-loop frequency;
- `OPEN`, `MID`, `CLOSE`, `DEMO` and `STOP` behavior after any future firmware modifications.

## Next engineering steps

1. Correct deterministic source defects in a dedicated branch without mixing them with repository-structure work.
2. Reconstruct a clean MSP432P401R CCS project from the declared TI SDK/toolchain.
3. Build from a fresh workspace and record compiler output/warnings.
4. Re-test the DFRobot platform on hardware before merging hardware-affecting fixes.
5. Only after successful re-validation, update the README from “originally demonstrated” to include newly reproduced evidence.
