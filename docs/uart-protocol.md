# UART command protocol

The reviewed UTC firmware uses newline-terminated text commands over UART at 9600 baud.

| Command | Purpose |
|---|---|
| `HELLO` | UART communication test |
| `*` | Identification/test command |
| `WHOAMI` | Read board identification |
| `POS` | Read four actuator positions |
| `STATUS` | Read board status |
| `OPEN` | Move to open reference |
| `MID` | Move to middle reference |
| `CLOSE` | Move to closed reference |
| `DEMO` | Execute the predefined sequence |
| `STOP` | Stop motors when the command loop is able to service the command |
| `HELP` | Print supported commands |

## Safety wording

`STOP` is a software command, not an independently validated emergency stop. The reviewed movement functions are blocking, so the architecture does not guarantee immediate asynchronous interruption while a movement routine is active.
