#include "game2.h"
#include "./pause_screen.h"
#include "gameState.h"
#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"

Graphics_Rectangle getRectangle(int x, int y)
{
    Graphics_Rectangle rect;
    rect.xMin = x - 2;
    rect.xMax = x + 2;
    rect.yMin = y - 2;
    rect.yMax = y + 2;
    return rect;
}

bool isOverlapping(const Graphics_Rectangle *rect1, const Graphics_Rectangle *rect2)
{
    if (
        rect1->xMax >= rect2->xMin && rect1->xMax <= rect2->xMax &&
        rect1->yMax >= rect2->yMin && rect1->yMax <= rect2->yMax)
    {
        return true;
    }
    else if (
        rect1->xMin >= rect2->xMin && rect1->xMin <= rect2->xMax &&
        rect1->yMax >= rect2->yMin && rect1->yMax <= rect2->yMax)
    {
        return true;
    }
    else if (
        rect1->xMin >= rect2->xMin && rect1->xMin <= rect2->xMax &&
        rect1->yMin >= rect2->yMin && rect1->yMin <= rect2->yMax)
    {
        return true;
    }
    else if (
        rect1->xMax >= rect2->xMin && rect1->xMax <= rect2->xMax &&
        rect1->yMin >= rect2->yMin && rect1->yMin <= rect2->yMax)
    {
        return true;
    }
    return false;
}

bool game2()
{
    GrClearDisplay(&gameState.gContext);
    bool alive = true;
    bool won = false;
    gameState.topButtonClicked = false;
    const int SPEED = 5;

    Graphics_Context *ctxPtr = &gameState.gContext;
    bool justResumed = true;
    bool justAteFruit = true;

    int snakeLength = 1;
    int headIndex = 0;
    int tailIndex = 0;
    int segmentsX[MAX_SNAKE_LEN];
    int segmentsY[MAX_SNAKE_LEN];
    int appleX = -1;
    int appleY = -1;

    segmentsX[0] = LCD_HORIZONTAL_MAX / 2;
    segmentsY[0] = LCD_VERTICAL_MAX - 1;
    enum direction_t direction = UP;
    enum direction_t lastMoved = UP;

    int i = 0;
    int j = 0;
    while (alive && !won)
    {
        j++;
        if (gameState.topButtonClicked)
        {
            pauseScreen();
            justResumed = true;
        }
        else
        {
            ADC14->CTL0 |= ADC14_CTL0_SC;
            uint32_t previousFg = ctxPtr->foreground;
            { //Update player direction, if necessary
                int lStrength = 0;
                int rStrength = 0;
                int uStrength = 0;
                int dStrength = 0;
                //Check if we need to move horizontally
                if (gameState.joystickX < J_LEFT_TRESH)
                {
                    lStrength = J_LEFT_TRESH - gameState.joystickX;
                }
                else if (gameState.joystickX > J_RIGHT_TRESH)
                {
                    rStrength = gameState.joystickX - J_RIGHT_TRESH;
                }
                //Check if we need to move vertically
                if (gameState.joystickY < J_DOWN_TRESH)
                {
                    dStrength = J_DOWN_TRESH - gameState.joystickY;
                }
                else if (gameState.joystickY > J_UP_TRESH)
                {
                    uStrength = gameState.joystickY - J_UP_TRESH;
                }

                if (uStrength > 0)
                {
                    if (rStrength > uStrength && lastMoved != LEFT)
                    {
                        direction = RIGHT;
                    }
                    else if (lStrength > uStrength && lastMoved != RIGHT)
                    {
                        direction = LEFT;
                    }
                    else if (lastMoved != DOWN)
                    {
                        direction = UP;
                    }
                }
                else if (dStrength > 0)
                {
                    if (rStrength > dStrength && lastMoved != LEFT)
                    {
                        direction = RIGHT;
                    }
                    else if (lStrength > dStrength && lastMoved != RIGHT)
                    {
                        direction = LEFT;
                    }
                    else if (lastMoved != UP)
                    {
                        direction = DOWN;
                    }
                }
                else if (rStrength > 0 && lastMoved != LEFT)
                {
                    direction = RIGHT;
                }
                else if (lStrength > 0 && lastMoved != RIGHT)
                {
                    direction = LEFT;
                }
            }
            if (j == 40)
            {
                j = 0;
                { //Delete last segment if necessary
                    if (justAteFruit)
                    {
                        justAteFruit = false;
                    }
                    else
                    {
                        //Erase last player segment
                        Graphics_Rectangle lastSegment = getRectangle(segmentsX[tailIndex], segmentsY[tailIndex]);
                        ctxPtr->foreground = ctxPtr->background;
                        Graphics_fillRectangle(ctxPtr, &lastSegment);
                    }
                }
                { //Move player
                    //Duplicate head in next cell
                    int prevHeadIndex = headIndex;
                    headIndex++;
                    headIndex %= MAX_SNAKE_LEN;
                    tailIndex++;
                    tailIndex %= MAX_SNAKE_LEN;
                    segmentsX[headIndex] = segmentsX[prevHeadIndex];
                    segmentsY[headIndex] = segmentsY[prevHeadIndex];
                    //Update head
                    switch (direction)
                    {
                    case UP:
                        segmentsY[headIndex] = segmentsY[headIndex] - SPEED;
                        lastMoved = UP;
                        break;
                    case DOWN:
                        segmentsY[headIndex] = segmentsY[headIndex] + SPEED;
                        lastMoved = DOWN;
                        break;
                    case RIGHT:
                        segmentsX[headIndex] = segmentsX[headIndex] + SPEED;
                        lastMoved = RIGHT;
                        break;
                    case LEFT:
                        segmentsX[headIndex] = segmentsX[headIndex] - SPEED;
                        lastMoved = LEFT;
                        break;
                    }
                    ctxPtr->foreground = previousFg;
                    //Draw head segment
                    Graphics_Rectangle headRect = getRectangle(segmentsX[headIndex], segmentsY[headIndex]);
                    Graphics_fillRectangle(ctxPtr, &headRect);
                }
                { //Check wall collision
                    if (segmentsY[headIndex] < 2 || segmentsY[headIndex] > LCD_VERTICAL_MAX - 2)
                    {
                        alive = false;
                        continue;
                    }
                    if (segmentsX[headIndex] < 2 || segmentsX[headIndex] > LCD_HORIZONTAL_MAX - 2)
                    {
                        alive = false;
                        continue;
                    }
                }
                { //Spawn apple, else check for collision
                    Graphics_Rectangle appleRect = getRectangle(appleX, appleY);
                    if (appleX == -1 || justResumed)
                    {
                        if (appleX == -1)
                        {
                            bool valid;
                            do
                            {
                                valid = true;
                                //Generate new apple
                                appleX = rand() % (LCD_HORIZONTAL_MAX - 4) + 2;
                                appleY = rand() % (LCD_HORIZONTAL_MAX - 4) + 2;
                                appleRect = getRectangle(appleX, appleY);
                                int i;
                                for (i = 0; i < snakeLength; i++)
                                {
                                    int x = segmentsX[(tailIndex + i) % MAX_SNAKE_LEN];
                                    int y = segmentsY[(tailIndex + i) % MAX_SNAKE_LEN];
                                    if (Graphics_isPointWithinRectangle(&appleRect, x, y))
                                    {
                                        valid = false;
                                    }
                                }
                            } while (!valid);
                        }
                        Graphics_setForegroundColor(ctxPtr, GRAPHICS_COLOR_RED);
                        Graphics_fillRectangle(ctxPtr, &appleRect);
                    }
                    Graphics_Rectangle headRect = getRectangle(segmentsX[headIndex], segmentsY[headIndex]);
                    if (isOverlapping(&appleRect, &headRect))
                    { //Eat apple
                        appleX = -1;
                        ctxPtr->foreground = ctxPtr->background;
                        //Delete apple
                        Graphics_fillRectangle(ctxPtr, &appleRect);
                        //Redraw head
                        ctxPtr->foreground = previousFg;
                        Graphics_fillRectangle(ctxPtr, &headRect);

                        ctxPtr->foreground = previousFg;
                        snakeLength++;
                        if (snakeLength == MAX_SNAKE_LEN)
                        {
                            won = true;
                            continue;
                        }
                        justAteFruit = true;
                        tailIndex--;
                        tailIndex %= MAX_SNAKE_LEN;
                    }
                }
                { //Check for collision with itself
                    int i;
                    Graphics_Rectangle headRect = getRectangle(segmentsX[headIndex], segmentsY[headIndex]);
                    for (i = 0; i < snakeLength - 1; i++)
                    {
                        int x = segmentsX[(tailIndex + i) % MAX_SNAKE_LEN];
                        int y = segmentsY[(tailIndex + i) % MAX_SNAKE_LEN];
                        if (Graphics_isPointWithinRectangle(&headRect, x, y))
                        {
                            alive = false;
                        }
                    }
                }
                ctxPtr->foreground = previousFg;
            }
        }
        for (i = 0; i < 20000; i++)
            ;
    }
    return alive;
}