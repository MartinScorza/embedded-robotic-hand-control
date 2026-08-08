#!/usr/bin/env python3
"""Apply only the deterministic DFRobot fixes approved in Issue #3.

The helper refuses to edit unexpected source revisions. It is intended to be
used once on the dedicated fix branch and removed before merge.
"""

from __future__ import annotations

import subprocess
from pathlib import Path


MAIN = Path("firmware/dfrobot-hand/main.c")
FUNCTIONS = Path("firmware/dfrobot-hand/Fonctions.c")

EXPECTED_BLOBS = {
    MAIN: "cdab6f12204460c1b4a0a1571c4074fc2464b360",
    FUNCTIONS: "0321a1ac357b7dc1bac2a51a799cb45d0dd98c49",
}


def blob_sha(path: Path) -> str:
    return subprocess.check_output(["git", "hash-object", str(path)], text=True).strip()


def replace_once(text: str, old: str, new: str, label: str) -> str:
    count = text.count(old)
    if count != 1:
        raise RuntimeError(f"{label}: expected exactly one match, found {count}")
    return text.replace(old, new, 1)


def main() -> None:
    current = {path: blob_sha(path) for path in EXPECTED_BLOBS}

    # Idempotent exit if the branch already contains the intended fixes.
    if current[MAIN] != EXPECTED_BLOBS[MAIN] or current[FUNCTIONS] != EXPECTED_BLOBS[FUNCTIONS]:
        main_text = MAIN.read_text(encoding="utf-8")
        functions_text = FUNCTIONS.read_text(encoding="utf-8")
        if (
            'sprintf(string, "%3u%",' not in main_text
            and "P7->DIR  &= ~(BIT1|BIT2);" in main_text
            and main_text.index("GPIO_init();") < main_text.index("ActiveSensor = Read_ActiveSensor();")
            and "uint16_t joystick_x = JoystickBuffer[0];" in functions_text
            and "uint16_t fsr_value = FSR01Buffer[servo];" in functions_text
        ):
            print("DFRobot deterministic fixes already applied; nothing to do.")
            return
        raise RuntimeError(
            "Source revision does not match the audited baseline and does not match the expected fixed state."
        )

    main_text = MAIN.read_text(encoding="utf-8")
    functions_text = FUNCTIONS.read_text(encoding="utf-8")

    if main_text.count('sprintf(string, "%3u%",') != 3:
        raise RuntimeError("Expected exactly three malformed mode/status format strings")
    main_text = main_text.replace('sprintf(string, "%3u%",', 'sprintf(string, "%3u",')

    main_text = replace_once(
        main_text,
        "    P3->DIR  &= ~(BIT1|BIT2);   // inputs\n"
        "    P3->REN  |=  (BIT1|BIT2);   // resistance\n"
        "    P3->OUT  |=  (BIT1|BIT2);   // pull-up\n",
        "    P7->DIR  &= ~(BIT1|BIT2);   // inputs\n"
        "    P7->REN  |=  (BIT1|BIT2);   // resistance\n"
        "    P7->OUT  |=  (BIT1|BIT2);   // pull-up\n",
        "selector GPIO port correction",
    )

    main_text = replace_once(
        main_text,
        "    ActiveSensor = Read_ActiveSensor();\n\n"
        "    PWM_init();\n"
        "    GPIO_init();\n"
        "    LCD_init();\n",
        "    GPIO_init();\n"
        "    ActiveSensor = Read_ActiveSensor();\n\n"
        "    PWM_init();\n"
        "    LCD_init();\n",
        "selector initialization order",
    )

    functions_text = replace_once(
        functions_text,
        "JoystickPWM_t JoystickBuffer_to_pwm(void){\n\n"
        "    JoystickPWM_t pwm;\n"
        "    pwm.pwm_x = SERVO_MIN_US + (((int32_t)JoystickBuffer[0] - Joystick_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (Joystick_max - Joystick_min);\n"
        "    pwm.pwm_y = SERVO_MIN_US + (((int32_t)JoystickBuffer[1] - Joystick_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (Joystick_max - Joystick_min);\n\n"
        "    return pwm;\n"
        "}\n",
        "JoystickPWM_t JoystickBuffer_to_pwm(void){\n\n"
        "    JoystickPWM_t pwm;\n"
        "    uint16_t joystick_x = JoystickBuffer[0];\n"
        "    uint16_t joystick_y = JoystickBuffer[1];\n\n"
        "    if(joystick_x < Joystick_min)\n"
        "        joystick_x = Joystick_min;\n"
        "    else if(joystick_x > Joystick_max)\n"
        "        joystick_x = Joystick_max;\n\n"
        "    if(joystick_y < Joystick_min)\n"
        "        joystick_y = Joystick_min;\n"
        "    else if(joystick_y > Joystick_max)\n"
        "        joystick_y = Joystick_max;\n\n"
        "    pwm.pwm_x = SERVO_MIN_US + (((int32_t)joystick_x - Joystick_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (Joystick_max - Joystick_min);\n"
        "    pwm.pwm_y = SERVO_MIN_US + (((int32_t)joystick_y - Joystick_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (Joystick_max - Joystick_min);\n\n"
        "    return pwm;\n"
        "}\n",
        "joystick input clamping",
    )

    functions_text = replace_once(
        functions_text,
        "uint16_t FSR01_to_pwm(uint8_t servo){\n\n"
        "    uint16_t pwm;\n"
        "    pwm = SERVO_MIN_US + (((int32_t)(FSR01_max - FSR01Buffer[servo])) * (SERVO_MAX_US - SERVO_MIN_US)) / (FSR01_max - FSR01_min);\n"
        "    return pwm;\n"
        "}\n",
        "uint16_t FSR01_to_pwm(uint8_t servo){\n\n"
        "    uint16_t pwm;\n"
        "    uint16_t fsr_value = FSR01Buffer[servo];\n\n"
        "    if(fsr_value < FSR01_min)\n"
        "        fsr_value = FSR01_min;\n"
        "    else if(fsr_value > FSR01_max)\n"
        "        fsr_value = FSR01_max;\n\n"
        "    pwm = SERVO_MIN_US + (((int32_t)(FSR01_max - fsr_value)) * (SERVO_MAX_US - SERVO_MIN_US)) / (FSR01_max - FSR01_min);\n"
        "    return pwm;\n"
        "}\n",
        "FSR input clamping",
    )

    MAIN.write_text(main_text, encoding="utf-8", newline="\n")
    FUNCTIONS.write_text(functions_text, encoding="utf-8", newline="\n")

    print("Applied deterministic DFRobot fixes from Issue #3.")


if __name__ == "__main__":
    main()
