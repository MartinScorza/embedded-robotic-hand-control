# Reproducibility and validation status

This document records checks performed during the portfolio reconstruction. It separates source/build validation from physical hardware evidence.

## Current public-safe baseline

The repository contains:

- DFRobot application firmware;
- Python/Tkinter UART interface;
- public-safe architecture/hardware documentation;
- Texas Instruments LCD support files with original license notices.

The repository intentionally excludes:

- generated CCS build artifacts and local workspace configuration;
- internal UTC technical PDFs;
- UTC branding assets;
- board-specific UTC register-map / low-level interface source.

## Python interface validation

Local/static validation:

- `python -m py_compile python-interface/robotic_hand_gui.py`: **PASS**.

GitHub Actions validation:

- workflow: `Python interface checks`;
- runner: Ubuntu;
- Python: 3.11;
- dependency installation from `python-interface/requirements.txt`: **PASS**;
- `python -m pip check`: **PASS**;
- `import serial`: **PASS**;
- `python -m py_compile python-interface/robotic_hand_gui.py`: **PASS**.

This validates the host-side dependency/syntax path only; it does not validate a physical serial connection.

## DFRobot firmware hardening

Five deterministic findings from the original reviewed source were corrected:

1. selector GPIO is initialized before `Read_ActiveSensor()`;
2. selector direction/pull resistors now use P7.1/P7.2 consistently with the read path;
3. three malformed LCD mode/status format strings were corrected;
4. joystick inputs are clamped to their configured calibration range before servo mapping;
5. FSR inputs are clamped to their configured calibration range before servo mapping.

The changes intentionally did **not** modify PWM timing, ADC channels, FMA SPI logic, progressive movement architecture, or UTC-hand code.

## DFRobot clean-build reproduction

A complete clean build of the corrected DFRobot source was reproduced on **2026-08-08** in a fresh Ubuntu 22.04 GitHub Actions runner.

Official legacy TI packages used:

- SimpleLink MSP432P4 SDK `3.40.01.02`;
- TI ARM Compiler `20.2.7.LTS`.

Compiled sources:

- `firmware/dfrobot-hand/main.c`;
- `firmware/dfrobot-hand/Fonctions.c`;
- retained Crystalfontz/ST7735 LCD driver sources;
- SDK `system_msp432p401r.c`;
- SDK `startup_msp432p401r_ccs.c`.

Linked against:

- TI Graphics Library;
- MSP432P4 DriverLib;
- MSP432P401R SDK linker command file;
- TI C runtime library.

Results:

- source compilation: **PASS**;
- firmware link: **PASS**;
- non-empty `dfrobot.out`: **PASS**;
- user-specific absolute paths required: **no**;
- firmware compile/link warnings observed: **none**.

Successful clean-build run: GitHub Actions `31231778633`.

The temporary workflow used to perform the legacy build was removed after validation so the portfolio repository does not download hundreds of megabytes of legacy TI tooling on every change.

## Automated repository hygiene

The `Public release guard` runs on pull requests and pushes to `main`. It checks the tracked tree for the configured publication hazards, including generated CCS artifacts, archives, local user paths and UTC material intentionally kept outside the public portfolio scope.

The guard passed after the DFRobot firmware fixes and after removal of the temporary build workflow.

## Physical validation boundary

The following are supported by the **original academic report/presentation and demonstration**, but have not been newly reproduced during the portfolio reconstruction:

- five-channel servo operation on the physical DFRobot hand;
- joystick, FSR and FMA operating modes on the physical setup;
- LCD/button/buzzer behavior after the current source fixes;
- oscilloscope measurement of the current five PWM outputs;
- physical selector-switch wiring and behavior;
- UTC `OPEN`, `MID`, `CLOSE`, `DEMO` and `STOP` behavior;
- effective end-to-end UTC PI control-loop frequency.

Therefore the repository may state that the project was **demonstrated on physical hardware during the original academic work**, and that the current DFRobot source is **clean-build reproduced**, but it must not claim that the corrected portfolio revision has been reflashed or physically revalidated unless that test is performed later.

## Remaining engineering limitations

See [`known-limitations.md`](known-limitations.md). Important remaining items include:

- blocking progressive movement routines inside GPIO interrupt handling;
- lack of a robust FMA SPI timeout;
- lack of explicit button debounce;
- blocking UTC communication/control behavior;
- nominal rather than measured end-to-end UTC control-loop frequency.
