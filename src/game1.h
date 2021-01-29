#include <stdbool.h>

#include "gameState.h"
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#ifndef __GAME_1_H__
#define __GAME_1_H__

//Play game 1.
//Returns true if won, false if lost
bool game1()
{
    GrClearDisplay(&gameState.gContext);
    bool playing = true;
    gameState.topButtonClicked = false;

    int playerX = LCD_HORIZONTAL_MAX / 2;
    int playerY = LCD_VERTICAL_MAX - 2;

    int i=0;
    while (playing)
    {
        if (gameState.topButtonClicked)
        {
            //Save game state
            //Enter pause
            gameState.topButtonClicked = false;
            // while (!gameState.topButtonClicked)
            // {
            //     //Draw pause screen
            // }
            //Restore game
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
                playerY = min(playerY + 1, LCD_VERTICAL_MAX - 1);
            }
            else if (gameState.joystickY > J_UP_TRESH)
            {
                playerY = max(playerY - 1, 1);
            }
            Graphics_Context *ctxPtr = &gameState.gContext;
            int previousFg=ctxPtr->foreground;
            if(prevPlayerRect.xMin != playerX - 1 || prevPlayerRect.yMin != playerY - 1){
            //Erase old player character
            ctxPtr->foreground=ctxPtr->background;
            Graphics_fillRectangle(ctxPtr, &prevPlayerRect);
            ctxPtr->foreground=previousFg;
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
            ctxPtr->foreground=GRAPHICS_COLOR_GREEN;
            Graphics_fillRectangle(ctxPtr, &targetRect);
            ctxPtr->foreground=previousFg;
            for(i=0;i<20000;i++);
        }
    }
    return true;
}
#endif
