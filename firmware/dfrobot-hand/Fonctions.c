/*
 * Fonctions.c
 *
 *      Authors: Lorenzo MAZZANTE, Martín SCORZA
 */
#include <Fonctions.h>

Graphics_Context g_sContext;
volatile uint16_t JoystickBuffer[2];
volatile bool BuzzerBuffer = false;
volatile uint16_t FSR01Buffer[5];
volatile uint16_t FMABuffer;
volatile uint8_t FMAStatus = 0xFF;

volatile uint16_t ServoPWMBuffer[5] = {
    SERVO_DEFAULT_US, SERVO_DEFAULT_US, SERVO_DEFAULT_US, SERVO_DEFAULT_US, SERVO_DEFAULT_US
};
volatile uint16_t PositionBuffer[5];

void write_servo(uint8_t Nservomoteur, uint16_t pulse_us){
    uint16_t ticks;

    if(pulse_us < SERVO_MIN_US){
        pulse_us = SERVO_MIN_US;
    }
    if(pulse_us > SERVO_MAX_US){
        pulse_us = SERVO_MAX_US;
    }
    ticks = pulse_us * TICKS_PER_US;

    switch (Nservomoteur)
        {
            case 0:
                TIMER_A1->CCR[1] = ticks; break;
            case 1:
                TIMER_A1->CCR[2] = ticks; break;
            case 2:
                TIMER_A1->CCR[3] = ticks; break;
            case 3:
                TIMER_A1->CCR[4] = ticks; break;
            case 4:
                TIMER_A3->CCR[2] = ticks; break;
            default: break;
        }
    return;
}

void write_servo_progresif(uint8_t Nservomoteur, uint16_t start_us, uint16_t end_us, uint16_t step_us){
    uint16_t us;

    if(start_us < end_us){
        for(us = start_us; us <= end_us; us+=  step_us){
            write_servo(Nservomoteur,us);
            _delay_cycles(150000);              // attente de 150000 cycles avec MCLK = 1.5 Mhz donc delay de 100ms
        }
    }
    else if(start_us > end_us){
        for(us = start_us; us >= end_us; us-= step_us){
            write_servo(Nservomoteur,us);
            _delay_cycles(150000);              // attente de 150000 cycles avec MCLK = 1.5 Mhz donc delay de 100ms
        }
    }
    return;
}

void write_hand_progresif(uint16_t start_us, uint16_t end_us, uint16_t step_us){
    uint16_t us;

    if(start_us < end_us){
        for(us = start_us; us <= end_us; us+=  step_us){
            write_servo(0,us);
            write_servo(1,us);
            write_servo(2,us);
            write_servo(3,us);
            write_servo(4,us);
            _delay_cycles(150000);              // attente de 150000 cycles avec MCLK = 1.5 Mhz donc delay de 100ms
        }
    }
    else if(start_us > end_us){
        for(us = start_us; us >= end_us; us-= step_us){
            write_servo(0,us);
            write_servo(1,us);
            write_servo(2,us);
            write_servo(3,us);
            write_servo(4,us);
            _delay_cycles(150000);              // attente de 150000 cycles avec MCLK = 1.5 Mhz donc delay de 100ms
        }
    }
    return;
}

void Buzzer_set(bool on)
{
    if(on && !BuzzerBuffer)
    {
        P2->SEL0 |= BIT7;
        P2->SEL1 &= ~BIT7;
        TIMER_A0->CTL = TASSEL__SMCLK | ID__8 | MC__UP | TACLR;

        BuzzerBuffer = true;
    }
    else if(!on && BuzzerBuffer)
    {
        TIMER_A0->CTL = TASSEL__SMCLK | ID__8 | MC__STOP | TACLR;

        /* Volver P2.7 a GPIO en bajo */
        P2->SEL0 &= ~BIT7;
        P2->SEL1 &= ~BIT7;
        P2->DIR  |= BIT7;
        P2->OUT  &= ~BIT7;

        BuzzerBuffer = false;
    }
}


JoystickPWM_t JoystickBuffer_to_pwm(void){

    JoystickPWM_t pwm;
    uint16_t joystick_x = JoystickBuffer[0];
    uint16_t joystick_y = JoystickBuffer[1];

    if(joystick_x < Joystick_min)
        joystick_x = Joystick_min;
    else if(joystick_x > Joystick_max)
        joystick_x = Joystick_max;

    if(joystick_y < Joystick_min)
        joystick_y = Joystick_min;
    else if(joystick_y > Joystick_max)
        joystick_y = Joystick_max;

    pwm.pwm_x = SERVO_MIN_US + (((int32_t)joystick_x - Joystick_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (Joystick_max - Joystick_min);
    pwm.pwm_y = SERVO_MIN_US + (((int32_t)joystick_y - Joystick_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (Joystick_max - Joystick_min);

    return pwm;
}


uint16_t FSR01_to_pwm(uint8_t servo){

    uint16_t pwm;
    uint16_t fsr_value = FSR01Buffer[servo];

    if(fsr_value < FSR01_min)
        fsr_value = FSR01_min;
    else if(fsr_value > FSR01_max)
        fsr_value = FSR01_max;

    pwm = SERVO_MIN_US + (((int32_t)(FSR01_max - fsr_value)) * (SERVO_MAX_US - SERVO_MIN_US)) / (FSR01_max - FSR01_min);
    return pwm;
}

uint8_t FMA_SendAndReceive(uint8_t octet)
{
    EUSCI_B3->TXBUF = octet;

    while(!(EUSCI_B3->IFG & EUSCI_B_IFG_RXIFG));
    return EUSCI_B3->RXBUF;
}

uint16_t FMA_read_valeur(volatile  uint8_t * status)
{
    uint8_t byte1, byte2;
    uint16_t raw;

    P10->OUT &= ~BIT2;        //activation du sensor

    __delay_cycles(10);

    byte1 = FMA_SendAndReceive(0x00);
    byte2 = FMA_SendAndReceive(0x00);

    __delay_cycles(2);
    P10->OUT |= BIT2;         //desactivation du sensor

    if(status != 0)
    {
        *status = (byte1 >> 6) & 0x03;
    }

    /* byte1 = S1 S0 B13 B12 B11 B10 B9 B8
       byte2 = B7 B6 B5 B4 B3 B2 B1 B0 */
    raw = (((uint16_t)(byte1 & 0x3F)) << 8) | byte2;

    return raw;
}

uint16_t FMA_to_PWM(uint16_t FMA_value){

    if (FMA_value > FMA_max)
        FMA_value = FMA_max;
    else if (FMA_value < FMA_min)
        FMA_value = FMA_min;

    uint16_t pwm;
    pwm = SERVO_MIN_US + (((int32_t)FMA_value - FMA_min) * (SERVO_MAX_US - SERVO_MIN_US)) / (FMA_max - FMA_min);
    return pwm;

}

void write_LCD(void)
{
    char string[8];

    sprintf(string, "%3u%%", PositionBuffer[0]);
    Graphics_drawString(&g_sContext,
                        (int8_t *)string,
                        AUTO_STRING_LENGTH,
                        92, 50,
                        OPAQUE_TEXT);

    sprintf(string, "%3u%%", PositionBuffer[1]);
    Graphics_drawString(&g_sContext,
                        (int8_t *)string,
                        AUTO_STRING_LENGTH,
                        92, 60,
                        OPAQUE_TEXT);

    sprintf(string, "%3u%%", PositionBuffer[2]);
    Graphics_drawString(&g_sContext,
                        (int8_t *)string,
                        AUTO_STRING_LENGTH,
                        92, 70,
                        OPAQUE_TEXT);

    sprintf(string, "%3u%%", PositionBuffer[3]);
    Graphics_drawString(&g_sContext,
                        (int8_t *)string,
                        AUTO_STRING_LENGTH,
                        92, 80,
                        OPAQUE_TEXT);

    sprintf(string, "%3u%%", PositionBuffer[4]);
    Graphics_drawString(&g_sContext,
                        (int8_t *)string,
                        AUTO_STRING_LENGTH,
                        92, 90,
                        OPAQUE_TEXT);
}

void write_LCD_oneFinger(uint8_t finger)
{
    char string[8];

    switch(finger)
    {
        case 0:
            sprintf(string, "%3u%%", PositionBuffer[0]);
            Graphics_drawString(&g_sContext, (int8_t *)string,
                                AUTO_STRING_LENGTH, 88, 70, OPAQUE_TEXT);
            break;

        case 1:
            sprintf(string, "%3u%%", PositionBuffer[1]);
            Graphics_drawString(&g_sContext, (int8_t *)string,
                                AUTO_STRING_LENGTH, 88, 80, OPAQUE_TEXT);
            break;

        case 2:
            sprintf(string, "%3u%%", PositionBuffer[2]);
            Graphics_drawString(&g_sContext, (int8_t *)string,
                                AUTO_STRING_LENGTH, 88, 90, OPAQUE_TEXT);
            break;

        case 3:
            sprintf(string, "%3u%%", PositionBuffer[3]);
            Graphics_drawString(&g_sContext, (int8_t *)string,
                                AUTO_STRING_LENGTH, 88, 100, OPAQUE_TEXT);
            break;

        case 4:
            sprintf(string, "%3u%%", PositionBuffer[4]);
            Graphics_drawString(&g_sContext, (int8_t *)string,
                                AUTO_STRING_LENGTH, 88, 110, OPAQUE_TEXT);
            break;

        default:
            break;
    }
}
