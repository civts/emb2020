#include "gameState.h"
#include "ti/devices/msp432p4xx/inc/msp432p401r.h"
#include "menu.h"

//Vertical offset for the options
const int optionsOffset = 50;

const char *const options[] = {"Game 1", "Game 2"};
const int optionsLength = 2;

void showMenu()
{
    Graphics_Context *context = &gameState.gContext;
    Graphics_clearDisplay(context);
    int currentlySelected = 0;
    int stepBetweenOptions = context->font->height * 2;

    gameState.screenIAmIn = Settings;
    int previouslySelected = currentlySelected;
    gameState.topButtonClicked = false;
    while ((!gameState.topButtonClicked) || currentlySelected >= optionsLength)
    {
        //Trigger ADC to read joystick position
        ADC14->CTL0 |= ADC14_CTL0_SC;

        if (gameState.joystickY < J_DOWN_TRESH)
        {
            currentlySelected = min(currentlySelected + 1, optionsLength - 1);
        }
        else if (gameState.joystickY > J_UP_TRESH)
        {
            currentlySelected = max(currentlySelected - 1, 0);
        }

        _drawTitle();

        if (previouslySelected != currentlySelected)
        {
            _cleanSelectionRectangle(previouslySelected, stepBetweenOptions);
            previouslySelected = currentlySelected;
        }

        _drawSelectionRectangle(currentlySelected, stepBetweenOptions);

        _drawOptions(stepBetweenOptions);

        if (gameState.topButtonClicked)
        {
            if (currentlySelected == optionsLength)
            {
                //TODO: Toggle light/dark
                gameState.topButtonClicked = false;
            }
        }
    }
    gameState.topButtonClicked = false;
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
            (int8_t *)options[i],
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

void _cleanSelectionRectangle(const int selected, const int step)
{
    int fgColor = gameState.gContext.foreground;
    gameState.gContext.foreground = gameState.gContext.background;
    Graphics_Context *context = &gameState.gContext;
    const int pad = 20;
    Graphics_Rectangle rectangle;
    rectangle.xMin = pad;
    rectangle.xMax = context->display->width - pad;
    rectangle.yMin = optionsOffset + selected * step - step / 2;
    rectangle.yMax = optionsOffset + selected * step + step / 2;
    Graphics_drawRectangle(context, &rectangle);
    gameState.gContext.foreground = fgColor;
}
