#include "display.h"
#include "../gameState.h"

void clearDisplay()
{
    GrClearDisplay(&gameState.gContext);
}

uint32_t getForegroundColor()
{
    return gameState.gContext.foreground;
}

uint32_t getBackgroundColor()
{
    return gameState.gContext.background;
}

uint16_t getFontHeight()
{
    return gameState.gContext.font->height;
}

Graphics_Font *getFont()
{
    return gameState.gContext.font;
}

void setFont(const Graphics_Font *font)
{
    GrContextFontSet(&gameState.gContext, font);
}

void setForegroundColorTranslated(uint32_t color)
{
    gameState.gContext.foreground = color;
}

void setBackgroundColorTranslated(uint32_t color)
{
    gameState.gContext.background = color;
}

void setForegroundColor(uint32_t color)
{
    Graphics_setForegroundColor(&gameState.gContext, color);
}

void setBackgroundColor(uint32_t color)
{
    Graphics_setBackgroundColor(&gameState.gContext, color);
}

void drawRectangle(const Graphics_Rectangle *rectangle)
{
    Graphics_drawRectangle(&gameState.gContext, rectangle);
}

void fillRectangle(const Graphics_Rectangle *rectangle)
{
    Graphics_fillRectangle(&gameState.gContext, rectangle);
}

void fillCircle(int x, int y, int radius)
{
    Graphics_fillCircle(&gameState.gContext, x, y, radius);
}

void drawString(const char *string, uint32_t x, uint32_t y, bool redrawBackground)
{
    Graphics_drawString(
        &gameState.gContext,
        (int8_t *)string,
        AUTO_STRING_LENGTH,
        x,
        y,
        redrawBackground);
}

void drawStringCentered(const char *string, uint32_t x, uint32_t y, bool redrawBackground)
{
    Graphics_drawStringCentered(
        &gameState.gContext,
        (int8_t *)string,
        AUTO_STRING_LENGTH,
        x,
        y,
        redrawBackground);
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
