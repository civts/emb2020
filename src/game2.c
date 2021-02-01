#include "game2.h"
#include "./pause_screen.h"
#include "gameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include "utils.h"
#define OPT_LEN 10
#define MAXL 100

Graphics_Rectangle getRectangle(int x, int y)
{
    Graphics_Rectangle rect;
    rect.xMin = x - 2;
    rect.xMax = x + 2;
    rect.yMin = y - 2;
    rect.yMax = y + 2;
    return rect;
}
Graphics_Rectangle getDecorationRectangle(int x, int y)
{
    Graphics_Rectangle rect;
    rect.xMin = x - 1;
    rect.xMax = x + 1;
    rect.yMin = y - 1;
    rect.yMax = y + 1;
    return rect;
}

bool game2()
{
    int maxSnakeLength = chooseMaxLength();
    GrClearDisplay(&gameState.gContext);
    bool alive = true;
    bool won = false;
    gameState.topButtonClicked = false;
    const int SPEED = 5;

    Graphics_Context *ctxPtr = &gameState.gContext;
    bool justResumed = true;

    int snakeLength = 1;
    int headIndex = 0;
    int tailIndex = 0;
    int segmentsX[MAXL];
    int segmentsY[MAXL];
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
                { //Delete last segment
                        //Erase last player segment
                        Graphics_Rectangle lastSegment = getRectangle(
                            segmentsX[tailIndex],
                            segmentsY[tailIndex]);
                        ctxPtr->foreground = ctxPtr->background;
                        Graphics_fillRectangle(ctxPtr, &lastSegment);
                }
                { //Move player
                    //Duplicate head in next cell
                    int prevHeadIndex = headIndex;
                    { //Draw snake decoration
                        if (snakeLength > 1)
                        {
                            Graphics_Rectangle decoration = getDecorationRectangle(
                                segmentsX[headIndex],
                                segmentsY[headIndex]);
                            Graphics_drawRectangle(ctxPtr, &decoration);
                        }
                    }
                    headIndex++;
                    headIndex %= MAXL;
                    tailIndex++;
                    tailIndex %= MAXL;
                    segmentsX[headIndex] = segmentsX[prevHeadIndex];
                    segmentsY[headIndex] = segmentsY[prevHeadIndex];
                    //Update head
                    switch (direction)
                    {
                    case UP:
                        segmentsY[headIndex] -= SPEED;
                        lastMoved = UP;
                        break;
                    case DOWN:
                        segmentsY[headIndex] += SPEED;
                        lastMoved = DOWN;
                        break;
                    case RIGHT:
                        segmentsX[headIndex] += SPEED;
                        lastMoved = RIGHT;
                        break;
                    case LEFT:
                        segmentsX[headIndex] -= SPEED;
                        lastMoved = LEFT;
                        break;
                    }
                    ctxPtr->foreground = previousFg;
                    //Draw head segment
                    Graphics_Rectangle headRect = getRectangle(
                        segmentsX[headIndex],
                        segmentsY[headIndex]);
                    Graphics_fillRectangle(ctxPtr, &headRect);
                }
                { //Check wall collision
                    if (segmentsY[headIndex] < 2 ||
                        segmentsY[headIndex] > LCD_VERTICAL_MAX - 2)
                    {
                        alive = false;
                        continue;
                    }
                    if (segmentsX[headIndex] < 2 ||
                        segmentsX[headIndex] > LCD_HORIZONTAL_MAX - 2)
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
                                    int x = segmentsX[(tailIndex + i) % MAXL];
                                    int y = segmentsY[(tailIndex + i) % MAXL];
                                    Graphics_Rectangle r = getRectangle(x, y);
                                    if (isOverlapping(&appleRect, &r))
                                    {
                                        valid = false;
                                    }
                                }
                            } while (!valid);
                        }
                        Graphics_setForegroundColor(ctxPtr, GRAPHICS_COLOR_RED);
                        Graphics_fillRectangle(ctxPtr, &appleRect);
                    }
                    Graphics_Rectangle headRect = getRectangle(
                        segmentsX[headIndex],
                        segmentsY[headIndex]);
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
                        if (snakeLength == maxSnakeLength)
                        {
                            won = true;
                            continue;
                        }
                        tailIndex--;
                        tailIndex %= MAXL;
                    }
                }
                { //Check for collision with itself
                    int i;
                    Graphics_Rectangle headRect = getRectangle(
                        segmentsX[headIndex],
                        segmentsY[headIndex]);
                    for (i = 0; i < snakeLength - 1; i++)
                    {
                        int x = segmentsX[(tailIndex + i) % MAXL];
                        int y = segmentsY[(tailIndex + i) % MAXL];
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

int chooseMaxLength()
{
    Graphics_Context *ctx = &gameState.gContext;
    Graphics_clearDisplay(ctx);
    Graphics_drawStringCentered(ctx,
                                (int8_t *)"Choose max score",
                                AUTO_STRING_LENGTH,
                                LCD_HORIZONTAL_MAX / 2,
                                10,
                                true);
    int options[OPT_LEN];
    int i;
    for (i = 0; i < OPT_LEN; i++)
    {
        options[i] = 10 * (i + 1);
    }
    assert(options[OPT_LEN - 1] <= MAXL);
    int selected = 0;
    Graphics_Rectangle r;
    r.xMin = 0;
    r.xMax = LCD_HORIZONTAL_MAX;
    r.yMin = 40;
    r.yMax = 60;
    uint32_t previousFg = ctx->foreground;
    int j = 0;
    bool shouldRedraw = true;
    while (!gameState.topButtonClicked)
    {
        j++;
        ADC14->CTL0 |= ADC14_CTL0_SC;
        if (j == 10 || shouldRedraw)
        {
            j = 0;
            if (gameState.joystickY < J_DOWN_TRESH)
            {
                gameState.joystickY = J_DOWN_TRESH + 1;
                selected--;
                if (selected < 0)
                {
                    selected = OPT_LEN - 1;
                }
                shouldRedraw = true;
            }
            else if (gameState.joystickY > J_UP_TRESH)
            {
                gameState.joystickY = J_UP_TRESH - 1;
                selected++;
                selected %= OPT_LEN;
                shouldRedraw = true;
            }

            if (shouldRedraw)
            {
                ctx->foreground = ctx->background;
                Graphics_fillRectangle(ctx, &r);
                ctx->foreground = previousFg;
                char string[4];
                sprintf(string, "%3d", options[selected]);

                Graphics_drawStringCentered(
                    ctx,
                    (int8_t *)string,
                    AUTO_STRING_LENGTH,
                    LCD_HORIZONTAL_MAX / 2,
                    50,
                    false);
            }
        }
        shouldRedraw = false;
        for (i = 0; i < 40000; i++)
            ;
    }
    gameState.topButtonClicked = false;
    return options[selected];
}
