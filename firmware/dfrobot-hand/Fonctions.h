/*
 * Fonctions.h
 *
 *      Authors: Lorenzo MAZZANTE, Martín SCORZA
 */
#ifndef FONCTIONS_H_
#define FONCTIONS_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

#define SERVO_MIN_US 1000
#define SERVO_MAX_US 1600
#define SERVO_DEFAULT_US 1000
#define TICKS_PER_US 3 // 12 MHz / 4 = 3 MHz

#define Joystick_max 14000
#define Joystick_min 7700

#define FSR01_max 16000 //sensor sin apretar
#define FSR01_min 6000  //sensor apretado al maximo

#define FMA_max 4200
#define FMA_min 3100

// Graphic library context
extern Graphics_Context g_sContext;
// EDU MKII joystick buffer
extern volatile uint16_t JoystickBuffer[2];
// EDU MKII buzzer buffer
extern volatile bool BuzzerBuffer;
// FSR01 buffer
extern volatile uint16_t FSR01Buffer[5];
// FMA buffer
extern volatile uint16_t FMABuffer;
extern volatile uint8_t FMAStatus;

extern volatile uint16_t ServoPWMBuffer[5];
extern volatile uint16_t PositionBuffer[5];

typedef struct{
    uint16_t pwm_x;
    uint16_t pwm_y;
} JoystickPWM_t;

void write_servo(uint8_t Nservomoteur, uint16_t pulse_us);

void write_servo_progresif(uint8_t Nservomoteur, uint16_t start_us, uint16_t end_us, uint16_t step_us);

void write_hand_progresif(uint16_t start_us, uint16_t end_us, uint16_t step_us);

void Buzzer_set(bool on);

JoystickPWM_t JoystickBuffer_to_pwm(void);

uint16_t FSR01_to_pwm(uint8_t servo);

uint8_t FMA_SendAndReceive(uint8_t octet);

uint16_t FMA_read_valeur(volatile uint8_t * status);

uint16_t FMA_to_PWM(uint16_t FMA_value);

void write_LCD(void);

void write_LCD_oneFinger(uint8_t finger);

#endif /* FONCTIONS_H_ */
