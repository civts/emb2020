#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include <ti/grlib/grlib.h>
#include "gameState.h"
#include "hwInit.h"

void hwInit()
{
    // Halting WDT
    WDT_A_holdTimer();
    configure_ADC();
    // Disable master interrupts
    Interrupt_disableMaster();

    // Set the core voltage level to VCORE1
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    // Set 2 flash wait states for Flash bank 0 and 1
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    // Initialize Clock System
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    _graphicsInit();
    _initButton();
    __enable_irq();
}

//Initializes the graphics AND the graphics context pointer
void _graphicsInit()
{
    // Initializes display
    Crystalfontz128x128_Init();

    // Set default screen orientation
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initializes graphics context
    Graphics_Context *gContext = &gameState.gContext;
    Graphics_initContext(gContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);

    Graphics_setForegroundColor(gContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(gContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(gContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(gContext);
}

void _initButton(void)
{
    P5->SEL0 &= ~BIT1;
    P5->SEL0 &= ~BIT1;
    P5->DIR &= ~BIT1;
    P5->REN |= BIT1;
    P5->OUT |= BIT1;
    P5->IFG &= ~BIT1;
    P5->IES |= BIT1;
    P5->IE |= BIT1;
    NVIC_EnableIRQ(PORT5_IRQn);
}

void configure_ADC()
{
    /*enable ports*/

    //J3.26->P4.4
    //Pin 4.4, joystick y
    P4->SEL0 &= ~BIT4;
    P4->SEL1 &= ~BIT4;
    P4->DIR &= ~BIT4;
    P4->REN &= ~BIT4;
    P4->OUT |= BIT4;
    P4->IFG &= ~BIT4;
    P4->IES |= BIT4;
    P4->IE &= ~BIT4;

    //J1.2->P6.0
    //Pin 6.0 joystick x
    P6->SEL0 &= ~BIT0;
    P6->SEL1 &= ~BIT0;
    P6->DIR &= ~BIT0;
    P6->REN &= ~BIT0;
    P6->OUT |= BIT0;
    P6->IFG &= ~BIT0;
    P6->IES |= BIT0;
    P6->IE &= ~BIT0;

    /*configure adc*/
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; //disable conversions

    while (REF_A->CTL0 & REF_A_CTL0_GENBUSY)
        ;                                            //wait while generator is busy
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON; //enable internal 1.2V ref

    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_3 | ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP;
    ADC14->CTL1 |= ADC14_CTL1_CH2MAP | ADC14_CTL1_RES__14BIT;

    //y direction joystick A9
    ADC14->MCTL[1] |= ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_INCH_9 | ADC14_CTL1_CSTARTADD_MASK;
    ADC14->IER0 |= ADC14_IER0_IE1; //enable interrupts
    //x direction joystick A15
    ADC14->MCTL[2] |= ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_INCH_15 | ADC14_CTL1_CSTARTADD_MASK;
    ADC14->MCTL[2] |= ADC14_MCTLN_EOS; //make this the end of sequence
    ADC14->IER0 |= ADC14_IER0_IE2;     //enable interrupts

    while (!(REF_A->CTL0 & REF_A_CTL0_GENRDY))
        ;                          //wait for generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC; //enable conversions
    NVIC_EnableIRQ(ADC14_IRQn);
}

//interrupt handler for the Analog to Digital converter
void ADC14_IRQHandler(void)
{
    __disable_irq();
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG1)
    {
        gameState.joystickY = ADC14->MEM[1];
    }
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG2)
    {
        gameState.joystickX = ADC14->MEM[2];
    }
    __enable_irq();
}

//Interrupt handler for the top button on the boosterpack
void PORT5_IRQHandler(void)
{
    __disable_irq();
    if (P5->IFG & BIT1)
    {
        uint32_t i;
        for (i = 0; i < 10000; i++)
            ;
        if (!(P5->IN & BIT1))
        {
            gameState.topButtonClicked = true;
        }
        P5->IFG &= ~BIT1;
    }
    __enable_irq();
}
