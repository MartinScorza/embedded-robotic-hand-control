# DFRobot hand firmware

This directory contains the application-specific firmware reviewed from the academic project:

- `main.c` — system initialization, sensor-mode selection, ADC/PWM/GPIO configuration and interrupt handlers;
- `Fonctions.c/.h` — servo commands, progressive movements, sensor conversion, FMA SPI acquisition, LCD updates and buzzer control;
- `LcdDriver/` — Texas Instruments Crystalfontz/ST7735 LCD support retained with its original license notices.

## Original development environment

The original Code Composer Studio metadata referenced:

- MSP432P401R;
- Code Composer Studio 12.8.x;
- TI ARM Compiler 20.2.7.LTS;
- SimpleLink MSP432P4 SDK 3.40.01.02;
- XDS110 debug interface;
- the TI `boostxl_edumkii_joystick` example as the project origin/template.

The original repository also contained CCS-generated build output, local launch settings, startup/system/linker copies and absolute template paths. Those artifacts are intentionally not duplicated here. MSP432 startup/system/linker support is supplied from the declared TI SDK/toolchain during reconstruction.

## Reproducibility status

A clean build of the current DFRobot source was independently reproduced on **2026-08-08** using a fresh Ubuntu 22.04 environment with:

- SimpleLink MSP432P4 SDK `3.40.01.02`;
- TI ARM Compiler `20.2.7.LTS`.

The application sources, retained LCD driver files and SDK startup/system sources compiled successfully and linked to a non-empty `dfrobot.out` without relying on user-specific absolute paths. No firmware compile/link warnings were observed.

This validates **source/build reproducibility**. The corrected portfolio revision has not been claimed as newly flashed or physically re-tested on the robotic hand.

## Source hardening

Five deterministic audit findings were corrected before the clean build:

- selector GPIO initialization order;
- P3/P7 selector configuration mismatch;
- malformed LCD mode/status format strings;
- joystick calibration-range clamping;
- FSR calibration-range clamping.

Broader design limitations such as blocking movement routines inside GPIO interrupt handling and the lack of a robust FMA SPI timeout remain intentionally documented rather than silently refactored without hardware validation.

See [`../../docs/known-limitations.md`](../../docs/known-limitations.md) and [`../../docs/reproducibility-status.md`](../../docs/reproducibility-status.md).
