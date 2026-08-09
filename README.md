# Embedded Robotic Hand Control

[![Python interface checks](https://github.com/MartinScorza/embedded-robotic-hand-control/actions/workflows/python-interface-checks.yml/badge.svg)](https://github.com/MartinScorza/embedded-robotic-hand-control/actions/workflows/python-interface-checks.yml)
[![Public release guard](https://github.com/MartinScorza/embedded-robotic-hand-control/actions/workflows/public-release-guard.yml/badge.svg)](https://github.com/MartinScorza/embedded-robotic-hand-control/actions/workflows/public-release-guard.yml)

Academic embedded-systems and robotics project developed by a two-student team at Université de Technologie de Compiègne (UTC). The project integrates **two robotic hands with different control architectures** around an MSP432P401R microcontroller.

- **DFRobot hand:** five servomotors driven directly by MSP432 hardware PWM, with joystick, FSR and digital force-sensor inputs.
- **UTC hand:** Python PC interface → UART → MSP432 → PI position control → SPI → dedicated hand-interface board → four actuators.

> This repository documents an educational robotics prototype. It is not a medical device, a clinically validated prosthesis, or a safety-certified control system.

## What this project demonstrates

- embedded C development on the MSP432P401R;
- hardware PWM control of five servomotors using Timer_A;
- ADC14 acquisition from joystick and force-sensitive resistor inputs;
- SPI communication with a digital force sensor and a dedicated robotic-hand interface board;
- UART communication between a Python desktop application and embedded firmware;
- PI-based position control for a four-actuator hand architecture;
- GPIO interrupts, SysTick scheduling, LCD/button/buzzer integration and hardware-facing debugging;
- reconstruction and clean-build validation of a legacy embedded toolchain without user-specific paths.

**Architecture and evidence:** [DFRobot architecture](hardware/diagrams/dfrobot-architecture.md) · [UTC architecture](hardware/diagrams/utc-architecture.md) · [PI control loop](hardware/diagrams/pi-control-loop.md) · [Validation evidence](docs/validation.md) · [Reproducibility](docs/reproducibility-status.md) · [Known limitations](docs/known-limitations.md)

## Project status

The original academic project included a physical hardware demonstration. This portfolio repository preserves the public-safe parts of that work while separating verified implementation details from limitations and material intentionally kept outside the publication scope.

The DFRobot application firmware and Python interface are included. Board-specific UTC interface firmware is withheld because the original project references internal UTC technical documentation.

The DFRobot source has also been rebuilt from a clean environment using the original TI legacy toolchain versions. This establishes **source/build reproducibility**, but it does not constitute a new physical hardware validation.

## Architecture

### DFRobot hand

```text
Joystick ── ADC14 ─────────────┐
5 × FSR ── ADC14 ──────────────┤
FMA sensor ── SPI ─────────────┤
                               ▼
                         MSP432P401R
                               │
                ┌──────────────┼──────────────┐
                ▼              ▼              ▼
          5 × PWM outputs     LCD        buttons/buzzer
                │
                ▼
          5 servomotors
```

The MSP432 directly controls five servomotors using hardware PWM. The firmware also acquires joystick and FSR signals through ADC14, reads the digital FMA sensor over SPI, and integrates the LCD, buttons and buzzer.

The configured servo period is **20 ms (50 Hz)**, with command pulse widths constrained in the reviewed firmware to approximately **1000–1600 µs**.

See the [detailed DFRobot architecture diagram](hardware/diagrams/dfrobot-architecture.md).

### UTC hand

```text
Python / Tkinter GUI
        │
        │ UART, 9600 baud
        ▼
    MSP432P401R
 command parser + PI
        │
        │ SPI
        ▼
UTC interface board
        │
        ▼
   4 actuators
```

For the UTC hand, the MSP432 receives high-level commands from the Python application, reads actuator positions through the interface board, computes PI control commands, and exchanges motor-control values through SPI.

See the [UTC architecture](hardware/diagrams/utc-architecture.md) and [PI control-loop diagram](hardware/diagrams/pi-control-loop.md).

## Technologies

**Embedded:** C, MSP432P401R, Code Composer Studio, Timer_A, ADC14, GPIO, interrupts, SysTick, UART, SPI, PWM, PI control  
**PC interface:** Python, Tkinter, pyserial  
**Sensors / peripherals:** joystick, five FSR sensors, Honeywell FMA-series force sensor, LCD, push buttons and buzzer

## Two control approaches

| | DFRobot hand | UTC hand |
|---|---|---|
| Actuators | 5 servomotors | 4 actuators |
| MSP432-to-hand control | Direct PWM | SPI through interface board |
| Main input | Joystick / force sensors | PC UART commands |
| Position feedback used by MSP432 | No | Yes |
| Control strategy | Open-loop servo command | PI position control |
| Local interface | LCD, buttons, buzzer | Python desktop GUI |

## Demonstrated functionality

The final academic documentation reports a practical hardware demonstration.

### DFRobot platform

- PWM generation on five servo outputs;
- joystick-based control;
- five-FSR control mode;
- FMA-sensor control mode;
- predefined/progressive movements;
- LCD, button and buzzer integration.

### UTC platform

- PC-to-MSP432 communication over UART;
- MSP432-to-interface-board communication over SPI;
- board identification using the expected `0x2024` value;
- position and status reads;
- `OPEN`, `MID`, `CLOSE` and `DEMO` movements;
- PI-based position control scheduled from a nominal 200 Hz SysTick.

These validations are primarily functional and qualitative. The preserved project material does **not** provide quantitative benchmarks for positioning accuracy, repeatability, overshoot, settling time, communication error rate or measured end-to-end control-loop frequency, so no such metrics are claimed here.

## UART commands

The reviewed UTC firmware implements newline-terminated text commands at 9600 baud:

| Command | Function |
|---|---|
| `HELLO` | Basic UART communication test |
| `*` | Firmware identification test |
| `WHOAMI` | Read board identification |
| `POS` | Read actuator positions |
| `STATUS` | Read board status |
| `OPEN` | Move to predefined open pose |
| `MID` | Move to predefined middle pose |
| `CLOSE` | Move to predefined closed pose |
| `DEMO` | Run the predefined movement sequence |
| `STOP` | Stop motors when the command loop can process the request |
| `HELP` | Print available commands |

`STOP` is a software command and is **not** presented as an independently validated emergency stop.

## Repository structure

```text
firmware/
  dfrobot-hand/       Public-safe DFRobot application sources
  utc-hand/           Public-scope note for intentionally withheld low-level material
python-interface/     Tkinter / pyserial PC interface
hardware/             Pin mapping, requirements and original architecture diagrams
docs/                 Architecture, validation, protocol notes and known limitations
tests/                Reproducibility and future validation plan
```

## DFRobot hardware mapping

Five PWM outputs are configured in the reviewed firmware:

| Servo | MSP432 pin | Timer output |
|---|---|---|
| 1 | P7.7 | TA1.1 |
| 2 | P7.6 | TA1.2 |
| 3 | P7.5 | TA1.3 |
| 4 | P7.4 | TA1.4 |
| 5 | P8.2 | TA3.2 |

Analog inputs include joystick axes on A15/A9 and five FSR channels on A16–A20. See [`hardware/dfrobot-pinout.md`](hardware/dfrobot-pinout.md).

## Building the DFRobot firmware

The original CCS project metadata references:

- MSP432P401R;
- Code Composer Studio 12.8.x;
- TI ARM Compiler 20.2.7.LTS;
- SimpleLink MSP432P4 SDK 3.40.01.02;
- XDS110 debug interface.

### Reproduced clean build

A clean build was independently reproduced on **2026-08-08** using a fresh Ubuntu 22.04 GitHub Actions runner with the official TI legacy packages:

- SimpleLink MSP432P4 SDK `3.40.01.02`;
- TI ARM Compiler `20.2.7.LTS`.

The build compiled the two DFRobot application sources, the retained LCD driver sources, and the SDK startup/system sources. It then linked them against TI Graphics Library, MSP432 DriverLib, the SDK linker command file and libc.

Result:

- source compilation: **PASS**;
- firmware link: **PASS**;
- non-empty `dfrobot.out`: **PASS**;
- user-specific absolute build paths required: **no**;
- firmware compile/link warnings observed: **none**.

This validates build reproducibility of the current source tree. It does **not** claim that the current portfolio revision was reflashed or physically re-tested on the robotic hand.

### Reconstruction workflow

1. Install Code Composer Studio with MSP432 support.
2. Install SimpleLink MSP432P4 SDK `3.40.01.02` and TI ARM Compiler `20.2.7.LTS`, or an appropriately validated compatible toolchain.
3. Create a clean MSP432P401R project.
4. Add the application sources under `firmware/dfrobot-hand/` and the retained LCD driver files.
5. Use SDK-provided startup, system and linker support rather than user-specific copies from the original workspace.
6. Select the MSP432P401R target and XDS110 connection.
7. Clean and build the project.
8. Verify wiring and external actuator power before flashing or operating the hardware.

See [`docs/reproducibility-status.md`](docs/reproducibility-status.md) for the validation boundary.

## Running the Python interface

```bash
python -m venv .venv
# activate the environment for your OS
pip install -r python-interface/requirements.txt
python python-interface/robotic_hand_gui.py
```

The host-side Python interface is covered by GitHub Actions smoke checks that install its declared dependency, run `pip check`, verify the `pyserial` import and compile the GUI source with `py_compile`.

## Known limitations

- The DFRobot hand uses commanded servo PWM rather than measured finger-position feedback.
- FSR/FMA mappings are prototype-specific and are not documented as calibrated force measurements in physical units.
- Progressive movement routines still contain blocking delays inside GPIO interrupt handling.
- FMA SPI acquisition still uses a blocking wait without a robust communication timeout.
- No explicit button debounce mechanism was identified in the DFRobot firmware.
- PI gains for the UTC hand were tuned for the academic setup; no general performance claim is made.
- SysTick is configured nominally at 200 Hz, but the complete closed-loop execution rate was not independently measured.
- Several UTC communication/control operations are blocking.
- The reviewed UTC SPI implementation does not provide a complete transfer timeout mechanism.
- `STOP` is not an asynchronous emergency-stop implementation.
- Automated hardware-in-the-loop tests are not currently included.
- Full UTC board-interface source is intentionally withheld from the portfolio scope.

See [`docs/known-limitations.md`](docs/known-limitations.md) and [`docs/reproducibility-status.md`](docs/reproducibility-status.md).

## Team and contribution

This was a two-student academic project by:

- **Lorenzo Mazzante** — `@lorenzomazzante`
- **Martín Scorza** — `@MartinScorza`

The project was developed collaboratively by both students. The preserved source material does not provide a sufficiently reliable subsystem-by-subsystem contribution breakdown, so this repository does not assign exclusive ownership of specific firmware, hardware, control or interface components.

See [`AUTHORS.md`](AUTHORS.md).

## Third-party software and licensing

The DFRobot CCS project originated from a Texas Instruments BOOSTXL-EDUMKII joystick example. This repository retains the TI Crystalfontz/ST7735 LCD support files under their original license notices; MSP432 startup/system/linker support is expected from the TI SDK/toolchain and is not duplicated here.

Internal UTC technical documentation, the low-level board register/protocol implementation and the UTC logo are intentionally excluded from the portfolio repository.

The repository is not currently released under a repository-wide open-source license. See [`NOTICE.md`](NOTICE.md) and [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).
