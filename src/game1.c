#include "game1.h"
#include "gameState.h"
#include "pause_screen.h"
#include <stdlib.h>
#include "hw_dependent/joystick.h"
#include "hw_dependent/display.h"

//Play game 1.
//Returns true if won, false if lost
bool game1()
{
    clearDisplay();
    bool alive = true;
    bool won = false;
    int round = 0;
    bool newRoundFlag = false;
    gameState.buttonClicked = false;

    int playerX = DISPLAY_WIDTH / 2;
    int playerY = DISPLAY_HEIGHT - 2;

    int i = 0;
    bool justResumed = true;

    Graphics_Rectangle targetRect;
    targetRect.xMin = DISPLAY_WIDTH / 2 - 2;
    targetRect.xMax = DISPLAY_WIDTH / 2 + 2;
    targetRect.yMin = 0;
    targetRect.yMax = 4;

    int enemyX[ROUND_NUMBER];
    int enemyY[ROUND_NUMBER];
    bool goingLeft[ROUND_NUMBER];

    for (i = 0; i < ROUND_NUMBER; i++)
    {
        const int pad = 20;
        enemyX[i] = rand() % DISPLAY_WIDTH;
        enemyY[i] = rand() % (DISPLAY_HEIGHT - 2 * pad) + pad;
        goingLeft[i] = rand() % 2;
    }
    while (alive && !won)
    {
        if (gameState.buttonClicked)
        {
            pauseScreen();
            justResumed = true;
        }
        else
        {
            uint32_t previousFg = getForegroundColor();
            if (newRoundFlag || justResumed)
            {
                if (newRoundFlag)
                {
                    newRoundFlag = false;
                    //Delete old one
                    setForegroundColorTranslated(getBackgroundColor());
                    fillRectangle(&targetRect);
                    if (round > 0)
                    {
                        //Create new one
                        targetRect.xMin = rand() % (DISPLAY_WIDTH - 8) + 4;
                        targetRect.xMax = targetRect.xMin + 4;
                        if (round % 2)
                        {
                            targetRect.yMin = DISPLAY_HEIGHT - 5;
                            targetRect.yMax = DISPLAY_HEIGHT;
                        }
                        else
                        {
                            targetRect.yMin = 0;
                            targetRect.yMax = 4;
                        }
                    }
                }
                //Draw target area
                setForegroundColor(GRAPHICS_COLOR_GREEN);
                fillRectangle(&targetRect);
                setForegroundColorTranslated(previousFg);
            }
            readJoystickPosition();

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
                    playerX = min(playerX + 1, DISPLAY_WIDTH - 1);
                }
                //Check if we need to move vertically
                if (gameState.joystickY < J_DOWN_TRESH)
                {
                    playerY = min(playerY + 1, DISPLAY_HEIGHT - 2);
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
                    setForegroundColorTranslated(getBackgroundColor());
                    fillCircle(prevPlayerX, prevPlayerY, PLAYER_RADIUS);
                    setForegroundColorTranslated(previousFg);
                    //Draw player character
                    fillCircle(playerX, playerY, PLAYER_RADIUS);
                }
            }
            //Check if player won he round
            {
                Graphics_Rectangle playerRect;
                playerRect.xMin = playerX - 2;
                playerRect.xMax = playerX + 2;
                playerRect.yMin = playerY - 2;
                playerRect.yMax = playerY + 2;
                if (isOverlapping(&targetRect, &playerRect))
                { //Player got the rectangle
                    round++;
                    newRoundFlag = true;
                    //Erase old target area
                    setForegroundColor(getBackgroundColor());
                    fillRectangle(&targetRect);
                    if (round == ROUND_NUMBER)
                    {
                        won = true;
                        setForegroundColorTranslated(previousFg);
                        continue;
                    }
                }
            }
            //Move enemies, draw them, check collisions
            {
                //Each round, one enemy is added
                int activeEnemies = round;
                for (i = 0; i < activeEnemies; i++)
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
                        const int limitRight = DISPLAY_WIDTH - 2;
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
                    setForegroundColorTranslated(getBackgroundColor());
                    fillRectangle(&previousRect);
                    //Draw new
                    Graphics_Rectangle enemyRect;
                    enemyRect.xMin = enemyX[i] - 1;
                    enemyRect.xMax = enemyX[i] + 1;
                    enemyRect.yMin = enemyY[i] - 1;
                    enemyRect.yMax = enemyY[i] + 1;
                    setForegroundColor(GRAPHICS_COLOR_RED);
                    fillRectangle(&enemyRect);
                }
                //Check collisions
                for (i = 0; i < activeEnemies; i++)
                {
                    Graphics_Rectangle playerRect;
                    playerRect.xMin = playerX - 4;
                    playerRect.xMax = playerX + 4;
                    playerRect.yMin = playerY - 4;
                    playerRect.yMax = playerY + 4;
                    if (isPointWithinRectangle(&playerRect, enemyX[i], enemyY[i]))
                    {
                        alive = false;
                    };
                }
                //Restore color for next iteration
                setForegroundColorTranslated(previousFg);
                for (i = 0; i < 40000; i++)
                    ;
            }
            justResumed = false;
        }
    }
    return alive;
}
