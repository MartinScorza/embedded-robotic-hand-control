# Embedded Robotic Hand Control

Academic embedded-systems and robotics project developed by a two-student team at Université de Technologie de Compiègne (UTC). The project integrates **two robotic hands with different control architectures** around an MSP432P401R microcontroller.

- **DFRobot hand:** five servomotors driven directly by MSP432 hardware PWM, with joystick, FSR and digital force-sensor inputs.
- **UTC hand:** Python PC interface → UART → MSP432 → PI position control → SPI → dedicated hand-interface board → four actuators.

> This repository documents an educational robotics prototype. It is not a medical device, a clinically validated prosthesis, or a safety-certified control system.

## Project status

The original academic project included a physical hardware demonstration. This portfolio repository preserves the public-safe parts of that work while separating verified implementation details from limitations and material whose redistribution rights are still under review.

The DFRobot application firmware and Python interface are included. Board-specific UTC interface firmware is currently withheld because the original project references internal UTC technical documentation.

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
  utc-hand/           Publication-status placeholder pending IP review
python-interface/     Tkinter / pyserial PC interface
hardware/             Public-safe pin mapping and hardware notes
docs/                 Architecture, validation and known limitations
media/                Portfolio media plan / future original evidence
tests/                Reproducibility and validation plan
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

A clean MSP432 build from the reorganized public-safe source tree has **not yet been independently reproduced**.

Recommended reconstruction workflow:

1. Install Code Composer Studio with MSP432 support.
2. Install the corresponding SimpleLink MSP432P4 SDK/toolchain.
3. Create a clean MSP432P401R project.
4. Add the application sources under `firmware/dfrobot-hand/` and the retained LCD driver files.
5. Use SDK/toolchain-provided startup, system and linker support rather than user-specific copies from the original workspace.
6. Select the MSP432P401R target and XDS110 connection.
7. Clean and build the project.
8. Verify wiring and external actuator power before flashing or operating the hardware.

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
- PI gains for the UTC hand were tuned for the academic setup; no general performance claim is made.
- SysTick is configured nominally at 200 Hz, but the complete closed-loop execution rate was not independently measured.
- Several communication/control operations are blocking.
- The reviewed UTC SPI implementation does not provide a complete transfer timeout mechanism.
- `STOP` is not an asynchronous emergency-stop implementation.
- Automated MSP432 firmware and hardware-in-the-loop tests are not currently included.
- Full UTC board-interface source is withheld pending redistribution/provenance review.

See [`docs/known-limitations.md`](docs/known-limitations.md) and [`docs/reproducibility-status.md`](docs/reproducibility-status.md).

## Team and contribution

This was a two-student academic project by:

- **Lorenzo Mazzante** — `@lorenzomazzante`
- **Martín Scorza** — `@MartinScorza`

The project was completed with an **equal overall contribution (50/50)** from both students.

The preserved source material does not provide a sufficiently reliable subsystem-by-subsystem contribution breakdown. To avoid unsupported individual claims, this repository therefore treats the implementation as collaborative work rather than assigning exclusive ownership of specific firmware, hardware, control or interface components.

See [`AUTHORS.md`](AUTHORS.md).

## Third-party software and licensing

The DFRobot CCS project originated from a Texas Instruments BOOSTXL-EDUMKII joystick example. This repository retains the TI Crystalfontz/ST7735 LCD support files under their original license notices; MSP432 startup/system/linker support is expected from the TI SDK/toolchain and is not duplicated here.

Board-specific UTC material remains withheld while provenance and redistribution rights are reviewed.

The repository is not currently released under a repository-wide open-source license. See [`NOTICE.md`](NOTICE.md) and [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).
