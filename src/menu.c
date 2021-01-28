#include "gameState.h"

//Vertical offset for the options
const int optionsOffset = 50;

const int8_t **const options[] = {"Game 1", "Game 2"};
const int optionsLength = 2;

void showMenu()
{
    Graphics_Context *context = &gameState.gContext;
    int currentlySelected = 0;
    int stepBetweenOptions = context->font->height * 2;

    while (1)
    {
        _drawTitle();

        _drawSelectionRectangle(currentlySelected, stepBetweenOptions);

        _drawOptions(stepBetweenOptions);
    }
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
