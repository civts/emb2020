#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>
#include "gameState.h"
#include "hwInit.h"

void hwInit()
{
    // Halting WDT
    WDT_A_holdTimer();
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

//Interrupt handler for the top button on the boosterpack
void PORT5_IRQHandler(void)
{
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
}
