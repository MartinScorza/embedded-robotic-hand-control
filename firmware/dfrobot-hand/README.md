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

The original repository also contained CCS-generated build output, local launch settings, startup/system/linker copies and absolute template paths. Those artifacts are intentionally not duplicated here. The MSP432 startup/system/linker support should be supplied from the installed TI SDK/toolchain when a clean portable project configuration is created.

## Reproducibility status

A clean build from this reorganized source tree has not yet been independently reproduced. The next engineering step is to create/import a clean MSP432P401R CCS project against the declared SDK, add these application sources, build without relying on user-specific paths, and revalidate on hardware.

No functional source fixes were applied during this repository-structure stage. Known findings from the code audit are documented in [`../../docs/known-limitations.md`](../../docs/known-limitations.md).
