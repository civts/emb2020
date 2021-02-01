#include "gameState.h"
#include "ti/devices/msp432p4xx/inc/msp432p401r.h"
#include "menu.h"
#include "../LightSensor/HAL_OPT3001.h"
#include <stdio.h>

//Vertical offset for the options
const int optionsOffset = 60;

const char *const options[] = {"Game 1", "Snake"};
const int optionsLength = 2;

void showMenu()
{
    Graphics_Context *context = &gameState.gContext;
    Graphics_clearDisplay(context);
    int currentlySelected = 0;
    int stepBetweenOptions = context->font->height * 2;

    int previouslySelected = currentlySelected;
    gameState.topButtonClicked = false;
    bool previouslyDark = false;
    Graphics_Rectangle previousRect;
    previousRect.xMin = 0;
    previousRect.xMax = 0;
    previousRect.yMin = 0;
    previousRect.yMax = 0;
    while ((!gameState.topButtonClicked) || currentlySelected >= optionsLength)
    {
        //Trigger ADC to read joystick position
        ADC14->CTL0 |= ADC14_CTL0_SC;

        adjustBrightness(&previouslyDark);

        if (gameState.joystickY < J_DOWN_TRESH)
        {
            currentlySelected = min(currentlySelected + 1, optionsLength);
            gameState.joystickY = J_DOWN_TRESH + 1;
            int i;
            for (i = 0; i < 400000; i++)
                ;
        }
        else if (gameState.joystickY > J_UP_TRESH)
        {
            currentlySelected = max(currentlySelected - 1, 0);
            gameState.joystickY = J_UP_TRESH - 1;
            int i;
            for (i = 0; i < 400000; i++)
                ;
        }

        _drawTitle();

        if (previouslySelected != currentlySelected)
        {
            _cleanRectangle(&previousRect);
            previouslySelected = currentlySelected;
        }

        previousRect = _drawSelectionRectangle(currentlySelected, stepBetweenOptions);

        _drawOptions(stepBetweenOptions);

        _drawLightDarkSwitch();

        if (gameState.topButtonClicked)
        {
            if (currentlySelected == optionsLength)
            {
                gameState.brightness++;
                gameState.brightness %= 3;
                gameState.topButtonClicked = false;
            }
        }
    }
    gameState.topButtonClicked = false;
    gameState.selectedGame = currentlySelected;
}

void _drawTitle()
{
    Graphics_Context *ctx = &gameState.gContext;
    uint32_t previousColor = ctx->foreground;
    Graphics_setForegroundColor(ctx, GRAPHICS_COLOR_GREEN);
    int width = ctx->display->width;

    const Graphics_Font *previousFont = ctx->font;
    GrContextFontSet(ctx, &g_sFontCm16b);
    Graphics_drawStringCentered(
        ctx,
        (int8_t *)"MSP Games",
        AUTO_STRING_LENGTH,
        width / 2,
        22,
        false);
    GrContextFontSet(ctx, previousFont);
    ctx->foreground = previousColor;
}

void _drawOptions(int step)
{
    Graphics_Context *ctx = &gameState.gContext;
    int halfWidth = ctx->display->width / 2;
    int yOffset = optionsOffset;
    int i;
    for (i = 0; i < optionsLength; i++)
    {
        Graphics_drawStringCentered(
            ctx,
            (int8_t *)options[i],
            AUTO_STRING_LENGTH,
            halfWidth,
            yOffset,
            false);
        yOffset += step;
    }
}

Graphics_Rectangle _drawSelectionRectangle(const int selected, const int step)
{
    Graphics_Context *context = &gameState.gContext;
    const int pad = 20;
    Graphics_Rectangle rectangle;
    rectangle.xMin = pad;
    rectangle.xMax = context->display->width - pad;
    rectangle.yMin = optionsOffset + selected * step - step / 2;
    rectangle.yMax = optionsOffset + selected * step + step / 2;

    if (selected == optionsLength)
    {
        rectangle.xMin = context->display->width - 20;
        rectangle.xMax = context->display->width - 1;
        rectangle.yMin = context->display->heigth - 15;
        rectangle.yMax = context->display->heigth - 2;
    }
    Graphics_drawRectangle(context, &rectangle);
    return rectangle;
}

void _cleanRectangle(const Graphics_Rectangle *rect)
{
    uint32_t fgColor = gameState.gContext.foreground;
    gameState.gContext.foreground = gameState.gContext.background;
    Graphics_drawRectangle(&gameState.gContext, rect);
    gameState.gContext.foreground = fgColor;
}

void _drawLightDarkSwitch()
{
    Graphics_Context *context = &gameState.gContext;
    char str[2];
    str[1] = '\0';
    switch (gameState.brightness)
    {
    case LIGHT:
        str[0] = 'L';
        break;
    case DARK:
        str[0] = 'D';
        break;
    case AUTO:
        str[0] = 'A';
        break;
    }
    Graphics_drawString(context,
                        (int8_t *)str,
                        AUTO_STRING_LENGTH,
                        context->display->width - 12,
                        context->display->heigth - context->font->height - 3,
                        true);
}

void adjustBrightness(bool *previouslyDark)
{
    Graphics_Context *context = &gameState.gContext;
    bool useDark = previouslyDark;
    switch (gameState.brightness)
    {
    case AUTO:
    { //Get luminosity
        unsigned long int lux = OPT3001_getLux();

        if (lux > 20)
        {
            useDark = false;
        }
        else if (lux < 10)
        {
            useDark = true;
        }
        break;
    }
    case DARK:
    {
        useDark = true;
        break;
    }
    case LIGHT:
    {
        useDark = false;
    }
    }
    if (useDark)
    {
        if (!(*previouslyDark))
        {
            Graphics_setBackgroundColor(context, 0x00232323);
            Graphics_setForegroundColor(context, GRAPHICS_COLOR_ANTIQUE_WHITE);
            *previouslyDark = true;
            Graphics_clearDisplay(context);
        }
    }
    else
    {
        if (*previouslyDark)
        {
            Graphics_setBackgroundColor(context, GRAPHICS_COLOR_WHITE);
            Graphics_setForegroundColor(context, GRAPHICS_COLOR_BLACK);
            *previouslyDark = false;
            Graphics_clearDisplay(context);
        }
    }
}
