#include "pause_screen.h"
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include "gameState.h"

void pauseScreen()
{
    Graphics_Context *ctxPtr = &gameState.gContext;
    //Enter pause
    Graphics_clearDisplay(ctxPtr);
    gameState.topButtonClicked = false;
    while (!gameState.topButtonClicked)
    {
        //Draw pause screen
        Graphics_drawStringCentered(
            ctxPtr,
            (int8_t *)"PAUSE",
            AUTO_STRING_LENGTH,
            LCD_HORIZONTAL_MAX / 2,
            20,
            false);
        Graphics_drawStringCentered(
            ctxPtr,
            (int8_t *)"Press S1 to resume",
            AUTO_STRING_LENGTH,
            LCD_HORIZONTAL_MAX / 2,
            80,
            false);
        //Wait for button
        __sleep();
    }
    gameState.topButtonClicked = false;
    Graphics_clearDisplay(ctxPtr);
}