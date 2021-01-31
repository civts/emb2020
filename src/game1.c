#include "game1.h"
#include "gameState.h"
#include "./pause_screen.h"
#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"

//Play game 1.
//Returns true if won, false if lost
bool game1()
{
    GrClearDisplay(&gameState.gContext);
    bool alive = true;
    bool won = false;
    gameState.topButtonClicked = false;

    int playerX = LCD_HORIZONTAL_MAX / 2;
    int playerY = LCD_VERTICAL_MAX - 2;

    int i = 0;
    Graphics_Context *ctxPtr = &gameState.gContext;
    bool justResumed = true;

    int enemyX[ENEMIES_COUNT];
    int enemyY[ENEMIES_COUNT];
    bool goingLeft[ENEMIES_COUNT];

    for (i = 0; i < ENEMIES_COUNT; i++)
    {
        const int pad = 20;
        enemyX[i] = rand() % LCD_HORIZONTAL_MAX;
        enemyY[i] = rand() % (LCD_VERTICAL_MAX - 2 * pad) + pad;
        goingLeft[i] = rand() % 2;
    }
    i = 0;
    while (alive && !won)
    {
        if (gameState.topButtonClicked)
        {
            pauseScreen();
            justResumed = true;
        }
        else
        {
            ADC14->CTL0 |= ADC14_CTL0_SC;
            uint32_t previousFg = ctxPtr->foreground;
            //Move and draw player
            {
                //Get old player position
                int prevPlayerX = playerX;
                int prevPlayerY = playerY;
                //Check if we need to move horizontally
                if (gameState.joystickX < J_LEFT_TRESH)
                {
                    playerX = max(playerX - 1, 1);
                }
                else if (gameState.joystickX > J_RIGHT_TRESH)
                {
                    playerX = min(playerX + 1, LCD_HORIZONTAL_MAX - 1);
                }
                //Check if we need to move vertically
                if (gameState.joystickY < J_DOWN_TRESH)
                {
                    playerY = min(playerY + 1, LCD_VERTICAL_MAX - 2);
                }
                else if (gameState.joystickY > J_UP_TRESH)
                {
                    playerY = max(playerY - 1, 1);
                }
                bool differentX = prevPlayerX != playerX;
                bool differentY = prevPlayerY != playerY;
                //If we have moved, redraw
                if (differentX || differentY || justResumed)
                {
                    //Erase old player character
                    ctxPtr->foreground = ctxPtr->background;
                    Graphics_fillCircle(ctxPtr, prevPlayerX, prevPlayerY, PLAYER_RADIUS);
                    ctxPtr->foreground = previousFg;
                    //Draw player character
                    Graphics_fillCircle(ctxPtr, playerX, playerY, PLAYER_RADIUS);
                }
            }
            //Draw target area, check if player won
            {
                Graphics_Rectangle targetRect;
                targetRect.xMin = LCD_HORIZONTAL_MAX / 2 - 2;
                targetRect.xMax = LCD_HORIZONTAL_MAX / 2 + 2;
                targetRect.yMin = 0;
                targetRect.yMax = 4;
                Graphics_setForegroundColor(ctxPtr, GRAPHICS_COLOR_GREEN);
                Graphics_fillRectangle(ctxPtr, &targetRect);
                if (Graphics_isPointWithinRectangle(&targetRect, playerX, playerY))
                {
                    won = true;
                    continue;
                }
            }
            //Move enemies, draw them, check collisions
            {
                for (i = 0; i < ENEMIES_COUNT; i++)
                {
                    Graphics_Rectangle previousRect;
                    previousRect.xMin = enemyX[i] - 1;
                    previousRect.xMax = enemyX[i] + 1;
                    previousRect.yMin = enemyY[i] - 1;
                    previousRect.yMax = enemyY[i] + 1;
                    //Move
                    if (goingLeft[i])
                    {
                        const int limitLeft = 2;
                        enemyX[i]--;
                        if (enemyX[i] < limitLeft)
                        {
                            enemyX[i] = limitLeft;
                            goingLeft[i] = false;
                        }
                        else
                        {
                            previousRect.xMin++;
                        }
                    }
                    else
                    {
                        const int limitRight = LCD_HORIZONTAL_MAX - 2;
                        enemyX[i]++;
                        if (enemyX[i] > limitRight)
                        {
                            enemyX[i] = limitRight;
                            goingLeft[i] = true;
                        }
                        else
                        {
                            previousRect.xMax--;
                        }
                    }
                    //Clear old one
                    ctxPtr->foreground = ctxPtr->background;
                    Graphics_fillRectangle(ctxPtr, &previousRect);
                    //Draw new
                    Graphics_Rectangle enemyRect;
                    enemyRect.xMin = enemyX[i] - 1;
                    enemyRect.xMax = enemyX[i] + 1;
                    enemyRect.yMin = enemyY[i] - 1;
                    enemyRect.yMax = enemyY[i] + 1;
                    Graphics_setForegroundColor(ctxPtr, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(ctxPtr, &enemyRect);
                }
                //Check collisions
                for (i = 0; i < ENEMIES_COUNT; i++)
                {
                    Graphics_Rectangle playerRect;
                    playerRect.xMin = playerX - 4;
                    playerRect.xMax = playerX + 4;
                    playerRect.yMin = playerY - 4;
                    playerRect.yMax = playerY + 4;
                    if (Graphics_isPointWithinRectangle(&playerRect, enemyX[i], enemyY[i]))
                    {
                        alive = false;
                    };
                }
                //Restore color for next iteration
                ctxPtr->foreground = previousFg;
                for (i = 0; i < 40000; i++)
                    ;
            }
        }
    }
    return alive;
}
