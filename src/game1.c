#include "game1.h"
#include "gameState.h"
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"

//Play game 1.
//Returns true if won, false if lost
bool game1()
{
    GrClearDisplay(&gameState.gContext);
    bool playing = true;
    gameState.topButtonClicked = false;

    int playerX = LCD_HORIZONTAL_MAX / 2;
    int playerY = LCD_VERTICAL_MAX - 2;

    int i = 0;
    Graphics_Context *ctxPtr = &gameState.gContext;
    bool justResumed = true;
    while (playing)
    {
        if (gameState.topButtonClicked)
        {
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
            justResumed = true;
            Graphics_clearDisplay(ctxPtr);
        }
        else
        {
            ADC14->CTL0 |= ADC14_CTL0_SC;
            Graphics_Rectangle prevPlayerRect;
            prevPlayerRect.xMin = playerX - 1;
            prevPlayerRect.xMax = playerX + 1;
            prevPlayerRect.yMin = playerY - 1;
            prevPlayerRect.yMax = playerY + 1;
            //Move
            if (gameState.joystickX < J_LEFT_TRESH)
            {
                playerX = max(playerX - 1, 1);
            }
            else if (gameState.joystickX > J_RIGHT_TRESH)
            {
                playerX = min(playerX + 1, LCD_HORIZONTAL_MAX - 1);
            }

            if (gameState.joystickY < J_DOWN_TRESH)
            {
                playerY = min(playerY + 1, LCD_VERTICAL_MAX - 2);
            }
            else if (gameState.joystickY > J_UP_TRESH)
            {
                playerY = max(playerY - 1, 1);
            }
            int previousFg = ctxPtr->foreground;
            bool differentXMin = prevPlayerRect.xMin != playerX - 1;
            bool differentYMin = prevPlayerRect.yMin != playerY - 1;
            if (differentXMin || differentYMin || justResumed)
            {
                //Erase old player character
                ctxPtr->foreground = ctxPtr->background;
                Graphics_fillRectangle(ctxPtr, &prevPlayerRect);
                ctxPtr->foreground = previousFg;
                //Draw player character
                Graphics_Rectangle playerRect;
                playerRect.xMin = playerX - 1;
                playerRect.xMax = playerX + 1;
                playerRect.yMin = playerY - 1;
                playerRect.yMax = playerY + 1;
                Graphics_fillRectangle(ctxPtr, &playerRect);
            }
            //Draw target area
            Graphics_Rectangle targetRect;
            targetRect.xMin = LCD_HORIZONTAL_MAX / 2 - 2;
            targetRect.xMax = LCD_HORIZONTAL_MAX / 2 + 2;
            targetRect.yMin = 0;
            targetRect.yMax = 4;
            Graphics_setForegroundColor(ctxPtr, GRAPHICS_COLOR_GREEN);
            Graphics_fillRectangle(ctxPtr, &targetRect);
            Graphics_setForegroundColor(ctxPtr, previousFg);
            for (i = 0; i < 40000; i++)
                ;
        }
    }
    return true;
}
