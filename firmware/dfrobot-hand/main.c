/*
 * main.c
 *
 *      Authors: Lorenzo MAZZANTE, Martín SCORZA
 */
#include <Fonctions.h>

typedef enum { //definitions des types de pilotage
    SENSOR_MODE_JOYSTICK = 0,
    SENSOR_MODE_FSR01    = 1,
    SENSOR_MODE_FMA      = 2
} SensorMode_t;

volatile SensorMode_t ActiveSensor = SENSOR_MODE_JOYSTICK; //capteur actif (par default: Joystick)
volatile uint8_t adcBusy = 0;
volatile uint8_t joystickReady = 0;
volatile uint8_t fsrReady = 0;
volatile uint8_t BuzzerRequest = 0;
volatile uint8_t PrintMode = 0;

void LCD_init(void){//initialization de l'ecran

    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"INTERFACE",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    10,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Homme - Machine",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    20,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Pilotage:",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    40,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Print Mode:",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    50,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Pouce: ",
                                    AUTO_STRING_LENGTH,
                                    50,
                                    70,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Index: ",
                                    AUTO_STRING_LENGTH,
                                    50,
                                    80,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Majeur: ",
                                    AUTO_STRING_LENGTH,
                                    50,
                                    90,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Annulaire: ",
                                    AUTO_STRING_LENGTH,
                                    50,
                                    100,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Auriculaire: ",
                                    AUTO_STRING_LENGTH,
                                    50,
                                    110,
                                    OPAQUE_TEXT);
    char string[8];

    sprintf(string, "%3u%", PrintMode);
    Graphics_drawString(&g_sContext, (int8_t *)string,
                        AUTO_STRING_LENGTH, 95, 50, OPAQUE_TEXT);

    sprintf(string, "%3u%", ActiveSensor);
    Graphics_drawString(&g_sContext, (int8_t *)string,
                        AUTO_STRING_LENGTH, 95, 40, OPAQUE_TEXT);
}

void Joystick_init(void){ //initialization de joystick edumkii

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, false); //adc multisequence: 2 signaux (X et Y)

    ADC14_configureConversionMemory(ADC_MEM0,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM1,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_clearInterruptFlag(ADC_INT1);
    ADC14_enableInterrupt(ADC_INT1);
}


void Buzzer_init(void){
    // Buzzer: P2.7 --> TA0.4

    // P2.7 como GPIO salida en 0
    P2->SEL0 &= ~BIT7;
    P2->SEL1 &= ~BIT7;
    P2->DIR  |= BIT7;
    P2->OUT  &= ~BIT7;

    // Configuración del timer, pero sin usar todavía el pin como timer
    TIMER_A0->CCTL[4] = OUTMOD_7;
    TIMER_A0->CCR[0]  = 2999; //1.5 MHz / (2999 + 1) = 500 Hz
    TIMER_A0->CCR[4]  = 1500; //1500 / 3000 = 50 %

    // Timer arrete au debut du programe
    TIMER_A0->CTL = TASSEL__SMCLK | ID__8 | MC__STOP | TACLR;
}


void PWM_init(){
    /*  servo1: P7.7 --> TA1.1
     *  servo2: P7.6 --> TA1.2
     *  servo3: P7.5 --> TA1.3
     *  servo4: P7.4 --> TA1.4
     *  servo5: P8.2 --> TA3.2
     */
    //Configuration de la broche associé avec la fonction de timer
    P7->SEL0 |= BIT7|BIT6|BIT5|BIT4;            //P7.7,P7.6,P7.5,P7.4 Configurés comme timers
    P7->SEL1 &= ~(BIT7|BIT6|BIT5|BIT4);         //P7.7,P7.6,P7.5,P7.4 Configurés comme timers
    P8->SEL0 |= BIT2;                           //P8.2 Configurés comme timers
    P8->SEL1 &= ~(BIT2);                        //P8.2 Configurés comme timers

    //Configuration de les broches associés en sortie
    P7->DIR |= BIT7|BIT6|BIT5|BIT4;
    P8->DIR |= BIT2;

    TIMER_A1->CCTL[1] = OUTMOD_7;  // TA1.1
    TIMER_A1->CCTL[2] = OUTMOD_7;  // TA1.2
    TIMER_A1->CCTL[3] = OUTMOD_7;  // TA1.3
    TIMER_A1->CCTL[4] = OUTMOD_7;  // TA1.4
    TIMER_A3->CCTL[2] = OUTMOD_7;  // TA3.2

    TIMER_A1->CCR[0]=59999; // 3MHz * 20ms -1
    TIMER_A3->CCR[0]=59999;

    TIMER_A1->CCR[1]=3000;  // 1000us * 3MHz
    TIMER_A1->CCR[2]=3000;
    TIMER_A1->CCR[3]=3000;
    TIMER_A1->CCR[4]=3000;
    TIMER_A3->CCR[2]=3000;

    TIMER_A1->CTL = TASSEL__SMCLK|ID__4|MC__UP|TACLR;
    TIMER_A3->CTL = TASSEL__SMCLK|ID__4|MC__UP|TACLR;
        //TASSEL__SMCLK --> horloge interne (à 12MHz)
        //ID__4 --> Divisor par 4
        //MC__UP --> Comptage en mode croissant
        //TACLR --> Reset le compteur
}


void GPIO_init(){
    /* buton 1: P1.1 (test1)
    *  buton 2: P1.4 (test2)
    */
    P1->SEL0 &= ~(BIT1 | BIT4);   //fonction 0 pour GPIO
    P1->SEL1 &= ~(BIT1 | BIT4);
    P1->DIR  &= ~(BIT1 | BIT4);   // inputs
    P1->REN  |=  (BIT1 | BIT4);   // resistance
    P1->OUT  |=  (BIT1 | BIT4);   // pull-up

    P1->IES |=  (BIT1 | BIT4);    //flanc descendent
    P1->IFG &= ~(BIT1 | BIT4);
    P1->IE  |=  (BIT1 | BIT4);

    /* buton 3: P3.5 (print mode)*/
    P3->SEL0 &= ~(BIT5);
    P3->SEL1 &= ~(BIT5);
    P3->DIR  &= ~(BIT5);   // inputs
    P3->REN  |=  (BIT5);   // resistance
    P3->OUT  |=  (BIT5);   // pull-up

    P3->IES |=  (BIT5);    //flanc descendent
    P3->IFG &= ~(BIT5);
    P3->IE  |=  (BIT5);

    /*GPIO pour ActiveSensor select: P7.1 et P7.2 */
    P7->SEL0 &= ~(BIT1|BIT2);
    P7->SEL1 &= ~(BIT1|BIT2);
    P3->DIR  &= ~(BIT1|BIT2);   // inputs
    P3->REN  |=  (BIT1|BIT2);   // resistance
    P3->OUT  |=  (BIT1|BIT2);   // pull-up

}


void FSR01_init(void)
{
    /*  FSR1: P9.1 --> A16
     *  FSR2: P9.0 --> A17
     *  FSR3: P8.7 --> A18
     *  FSR4: P8.6 --> A19
     *  FSR5: P8.5 --> A20
     */
    //Configuration des broches comme imputs
    P9->DIR &= ~(BIT1 | BIT0);
    P8->DIR &= ~(BIT7 | BIT6 | BIT5);
    //Configuration des broches en fonction 3 (analogique) --> ADC
    P9->SEL0 |= (BIT1 | BIT0);
    P8->SEL0 |= (BIT7 | BIT6 | BIT5);
    P9->SEL1 |= (BIT1 | BIT0);
    P8->SEL1 |= (BIT7 | BIT6 | BIT5);
    //ADC en lectrue multisequence: 5 signaux analogique (5 doigts)
    ADC14_configureMultiSequenceMode(ADC_MEM2, ADC_MEM6, false);

    ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A16, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A17, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A18, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM5, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A19, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM6, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A20, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_clearInterruptFlag(ADC_INT6);
    ADC14_enableInterrupt(ADC_INT6);
}

void ADC_init(void){//initialization global du module ADC14 (utilisé avec le mode joystick et FSR01)

    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    ADC14_setSampleHoldTrigger(ADC_TRIGGER_ADCSC, false);
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void FMA_init(void){
    /*
     * FMA VS   -> MSP432 3.3V
     * FMA GND  -> MSP432 GND
     * FMA SCLK -> MSP432 P10.1
     * FMA MISO -> MSP432 P10.3
     * FMA SS   -> MSP432 P10.2
     *
     * Digital clock frecuency: 50 - 800 kHz --> On choit f=500kHz
     *
    */

    //Configuration de P10.2 comme GPIO en sortie (SS)
    P10->SEL0 &= ~BIT2;
    P10->SEL1 &= ~BIT2;
    P10->DIR |= BIT2;
    P10->OUT |= BIT2;

    //Configuration de P10.1 comme fonction 1 en sortie (CLK)
    P10->SEL0 |= BIT1;
    P10->SEL1 &= ~BIT1;
    P10->DIR |= BIT1;

    //Configuration de P10.3 comme fonction 1 en entrée (MISO)
    P10->SEL0 |= BIT3;
    P10->SEL1 &= ~BIT3;
    P10->DIR &= ~BIT3;

    //Configuration du mode SPI MSP432
    EUSCI_B3->CTLW0 = EUSCI_B_CTLW0_SWRST;             // Mettre EUSCI_B3 en état reset

    EUSCI_B3->CTLW0 |=  EUSCI_B_CTLW0_MST       |       // Mode maitre
                        EUSCI_B_CTLW0_SYNC      |       // Mode synchrone (SPI)
                        EUSCI_B_CTLW0_CKPH      |       // Lecture sur front montant
                        EUSCI_B_CTLW0_MSB       |       // Envoi du MSB en premier
                        EUSCI_B_CTLW0_UCSSEL_2;         // horloge SPI : SMCLK

    EUSCI_B3->BRW = 24;                                 // frequence de SMCLK (12 Mhz) divisee par 24 (donc 500 kHz)

    EUSCI_B3->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;            // Mettre EUSCI_BO en etat actif

    // attendre un peu avant la première lecture
    __delay_cycles(5000);
}

void PWMBuffer_to_PositionBuffer(void) // fonction que transforme le valeur de PWM en pourcentage (0-100%)
{
    uint8_t i;
    int32_t pos;

    for(i = 0; i < 5; i++)
    {
        pos = ((int32_t)(ServoPWMBuffer[i] - SERVO_MIN_US) * 100) / (SERVO_MAX_US - SERVO_MIN_US);

        if(pos < 0)
            pos = 0;
        else if(pos > 100)
            pos = 100;

        PositionBuffer[i] = (uint16_t)pos;
    }
}

SensorMode_t Read_ActiveSensor(void) //fonction qui permet de lire le capteur a utiliser (a choisir manuelement)
{
    uint8_t s1, s2;

    s1 = (P7->IN & BIT1) ? 1 : 0;   // P7.1
    s2 = (P7->IN & BIT2) ? 1 : 0;   // P7.2

    if((s1 == 0) && (s2 == 0))
    {
        return SENSOR_MODE_JOYSTICK;
    }
    else if((s1 == 0) && (s2 == 1))
    {
        return SENSOR_MODE_FSR01;
    }
    else if((s1 == 1) && (s2 == 0))
    {
        return SENSOR_MODE_FMA;
    }
    else
    {
        return SENSOR_MODE_JOYSTICK;
    }
}

int main(void)
{
    // Stop watchdog
    MAP_WDT_A_holdTimer();

    // Disable global interrupts while configuring the system
    MAP_Interrupt_disableMaster();

    // VCORE1 required for high-frequency operation
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    // Flash wait states required for 48 MHz
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    // Set DCO = 48 MHz
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    // Clock Init
    MAP_CS_initClockSignal(CS_MCLK,   CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_32);  // 48/32 = 1.5 MHz
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);   // 48/4  = 12 MHz
    MAP_CS_initClockSignal(CS_SMCLK,  CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);   // 48/4  = 12 MHz
    MAP_CS_initClockSignal(CS_ACLK,   CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);  // 32.768 kHz

    ActiveSensor = Read_ActiveSensor();

    PWM_init();
    GPIO_init();
    LCD_init();

    switch(ActiveSensor)
    {
        case SENSOR_MODE_JOYSTICK:
            ADC_init();
            Buzzer_init();
            Joystick_init();
            break;

        case SENSOR_MODE_FSR01:
            ADC_init();
            FSR01_init();
            break;

        case SENSOR_MODE_FMA:
            FMA_init();
            break;

        default:
            break;
    }

    // Habilitate interruptions
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_EnableIRQ(PORT1_IRQn);
    NVIC_EnableIRQ(PORT3_IRQn);
    __enable_irq();

    if((ActiveSensor == SENSOR_MODE_JOYSTICK) || (ActiveSensor == SENSOR_MODE_FSR01)){
        ADC14_enableConversion();
        adcBusy = 1;
        ADC14_toggleConversionTrigger(); //on lance la premier conversion
    }

    uint16_t lcdCounter = 0;
    uint8_t lcdFingerIndex = 0;
    while(1)
    {
        switch(ActiveSensor)
            {
                case SENSOR_MODE_JOYSTICK:
                    if (joystickReady == 1) //si la lecture de joystick a fini --> on ecrit dans les servos
                    {
                        uint8_t i;
                        JoystickPWM_t pwm_value;
                        joystickReady = 0;

                        pwm_value = JoystickBuffer_to_pwm();

                        ServoPWMBuffer[0] = pwm_value.pwm_x;
                        ServoPWMBuffer[1] = pwm_value.pwm_y;
                        ServoPWMBuffer[2] = pwm_value.pwm_y;
                        ServoPWMBuffer[3] = pwm_value.pwm_y;
                        ServoPWMBuffer[4] = pwm_value.pwm_y;

                        for(i = 0; i < 5; i++)
                        {
                            write_servo(i, ServoPWMBuffer[i]);
                        }

                        BuzzerRequest = ((JoystickBuffer[0] > Joystick_max) || (JoystickBuffer[1] > Joystick_max));
                        if (BuzzerRequest != BuzzerBuffer) //si la valeur est maximale
                        {
                            Buzzer_set(BuzzerRequest); //on etteindre le buzzer
                        }
                    }

                    if (adcBusy == 0) //si le adc est libre on lance la prochaine conversion
                    {
                        adcBusy = 1;
                        ADC14_toggleConversionTrigger();
                    }
                    break;

                case SENSOR_MODE_FSR01:
                    if (fsrReady == 1) //si la lectures des capteurs FSR01 a fini --> on ecrit dans les servos
                    {
                        uint8_t i;
                        fsrReady = 0;

                        for(i = 0; i < 5; i++)
                        {
                            ServoPWMBuffer[i] = FSR01_to_pwm(i);
                            write_servo(i, ServoPWMBuffer[i]);
                        }
                    }

                    if (adcBusy == 0) //si le adc es libre, on lance la prochaine conversion
                    {
                        adcBusy = 1;
                        ADC14_toggleConversionTrigger();
                    }
                    break;

                case SENSOR_MODE_FMA:
                {
                    uint8_t i;

                    FMABuffer = FMA_read_valeur(&FMAStatus);

                    if(FMAStatus == 0x00)   //si la data est valide --> on ecrit dans les servos
                    {
                        uint16_t pwm = FMA_to_PWM(FMABuffer);
                        ServoPWMBuffer[0] = pwm;
                        ServoPWMBuffer[1] = pwm;
                        ServoPWMBuffer[2] = pwm;
                        ServoPWMBuffer[3] = pwm;
                        ServoPWMBuffer[4] = pwm;

                        for(i = 0; i < 5; i++)
                        {
                            write_servo(i, ServoPWMBuffer[i]);
                        }
                    }
                    break;
                }

                default:
                    break;
            }
        if(PrintMode){
            lcdCounter++;
            if(lcdCounter >= 2000)
            {
                lcdCounter = 0;
                PWMBuffer_to_PositionBuffer();
                write_LCD_oneFinger(lcdFingerIndex);

                lcdFingerIndex++;
                if(lcdFingerIndex >= 5)
                {
                    lcdFingerIndex = 0;
                }
            }
        }

    }
}

void PORT1_IRQHandler() //interrupcion par les buttons 1 et 2 (tests initiales)
{
    if (P1->IFG & BIT1)
    {
        write_hand_progresif(1000,1600,50);
        write_hand_progresif(1600,1000,50);
        P1->IFG &= ~BIT1;
    }

    if (P1->IFG & BIT4)
    {
        write_servo_progresif(0,1000,1600,50);
        _delay_cycles(75000);
        write_servo_progresif(1,1000,1600,50);
        _delay_cycles(75000);
        write_servo_progresif(2,1000,1600,50);
        _delay_cycles(75000);
        write_servo_progresif(3,1000,1600,50);
        _delay_cycles(75000);
        write_servo_progresif(4,1000,1600,50);
        _delay_cycles(75000);
        write_hand_progresif(1600,1000,50);
        P1->IFG &= ~BIT4;
    }
}

void PORT3_IRQHandler() //interrupcion par le button 3 (EDU MKII) --> il va regler le mode d'impression
{
    if (P3->IFG & BIT5)
    {
        if(PrintMode == 0)
            PrintMode = 1;
        else
            PrintMode = 0;
        char string[8];

        sprintf(string, "%3u%", PrintMode);
        Graphics_drawString(&g_sContext, (int8_t *)string,
                            AUTO_STRING_LENGTH, 95, 50, OPAQUE_TEXT);
        P3->IFG &= ~BIT5;
    }
}

void ADC14_IRQHandler(void) //interrupcion par le module ADC14 (pour lire les valeurs du joystick et des capteurs FSR01)
{
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if ((ActiveSensor == SENSOR_MODE_JOYSTICK) && (status & ADC_INT1))
    {
        JoystickBuffer[0] = ADC14_getResult(ADC_MEM0);
        JoystickBuffer[1] = ADC14_getResult(ADC_MEM1);

        joystickReady = 1;
        adcBusy = 0;
    }
    else if ((ActiveSensor == SENSOR_MODE_FSR01) && (status & ADC_INT6))
    {
        FSR01Buffer[0] = ADC14_getResult(ADC_MEM2);
        FSR01Buffer[1] = ADC14_getResult(ADC_MEM3);
        FSR01Buffer[2] = ADC14_getResult(ADC_MEM4);
        FSR01Buffer[3] = ADC14_getResult(ADC_MEM5);
        FSR01Buffer[4] = ADC14_getResult(ADC_MEM6);

        fsrReady = 1;
        adcBusy = 0;
    }
}
