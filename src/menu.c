#include "gameState.h"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <stdio.h>
#include "menu.h"

//Vertical offset for the options
const int optionsOffset = 50;

const int8_t **const options[] = {"Game 1", "Game 2"};
const int optionsLength = 2;

void showMenu()
{
    Graphics_Context *context = &gameState.gContext;
    int currentlySelected = 0;
    int stepBetweenOptions = context->font->height * 2;

    gameState.screenIAmIn = Settings;
    while ((!gameState.topButtonClicked) || currentlySelected >= optionsLength)
    {
        ADC14->CTL0 |= ADC14_CTL0_SC;
        _drawTitle();

        _drawSelectionRectangle(currentlySelected, stepBetweenOptions);

        _drawOptions(stepBetweenOptions);

        if (gameState.topButtonClicked)
        {
            if (currentlySelected == optionsLength)
            {
                //TODO: Toggle light/dark
            }
            gameState.topButtonClicked = false;
        }
        char string[10];
        sprintf(string, "X: %5d", gameState.joystickX);
        Graphics_drawStringCentered(&gameState.gContext,
                                    (int8_t *)string,
                                    8,
                                    64,
                                    100,
                                    OPAQUE_TEXT);
        //Wait for user input
    }
    gameState.selectedGame = currentlySelected;
    gameState.screenIAmIn = Changing;
}

void _drawTitle()
{
    Graphics_Context *ctx = &gameState.gContext;
    uint32_t previousColor = ctx->foreground;
    Graphics_setForegroundColor(ctx, GRAPHICS_COLOR_GREEN);
    int font_height = ctx->font->height;
    int width = ctx->display->width;
    Graphics_drawStringCentered(
        ctx,
        (int8_t *)"Game system 2",
        AUTO_STRING_LENGTH,
        width / 2,
        font_height / 2 + 2,
        false);
    Graphics_setForegroundColor(ctx, previousColor);
}

void _drawOptions(int step)
{
    Graphics_Context *ctx = &gameState.gContext;
    uint32_t previousColor = ctx->foreground;
    Graphics_setForegroundColor(ctx, GRAPHICS_COLOR_BLACK);
    int halfWidth = ctx->display->width / 2;
    int yOffset = optionsOffset;
    int i;
    for (i = 0; i < optionsLength; i++)
    {
        Graphics_drawStringCentered(
            ctx,
            options[i],
            AUTO_STRING_LENGTH,
            halfWidth,
            yOffset,
            false);
        yOffset += step;
    }
    Graphics_setForegroundColor(ctx, previousColor);
}

void _drawSelectionRectangle(const int selected, const int step)
{
    Graphics_Context *context = &gameState.gContext;
    const int pad = 20;
    Graphics_Rectangle rectangle;
    rectangle.xMin = pad;
    rectangle.xMax = context->display->width - pad;
    rectangle.yMin = optionsOffset + selected * step - step / 2;
    rectangle.yMax = optionsOffset + selected * step + step / 2;
    Graphics_drawRectangle(context, &rectangle);
}
