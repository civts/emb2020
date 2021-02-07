#include "gameState.h"
#include "menu.h"
#include <stdio.h>
#include "hw_dependent/display.h"
#include "hw_dependent/joystick.h"
#include "hw_dependent/light_sensor.h"

//Vertical offset for the options
const int optionsOffset = 60;

const char *const options[] = {"Game 1", "Snake"};
const int optionsLength = 2;

void showMenu()
{
    clearDisplay();
    int currentlySelected = 0;
    int stepBetweenOptions = getFontHeight() * 2;

    int previouslySelected = currentlySelected;
    gameState.buttonClicked = false;
    bool previouslyDark = false;
    Graphics_Rectangle previousRect;
    previousRect.xMin = 0;
    previousRect.xMax = 0;
    previousRect.yMin = 0;
    previousRect.yMax = 0;
    while ((!gameState.buttonClicked) || currentlySelected >= optionsLength)
    {
        readJoystickPosition();

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

        if (gameState.buttonClicked)
        {
            if (currentlySelected == optionsLength)
            {
                gameState.brightness++;
                gameState.brightness %= 3;
                gameState.buttonClicked = false;
            }
        }
    }
    gameState.buttonClicked = false;
    gameState.selectedGame = currentlySelected;
}

void _drawTitle()
{
    uint32_t previousColor = getForegroundColor();
    setForegroundColor(GRAPHICS_COLOR_GREEN);
    int width = DISPLAY_WIDTH;

    const Graphics_Font *previousFont = getFont();
    setFont(titleFont);
    drawStringCentered(
        "MSP Games",
        width / 2,
        22,
        false);
    setFont(previousFont);
    setForegroundColorTranslated(previousColor);
}

void _drawOptions(int step)
{
    int halfWidth = DISPLAY_WIDTH / 2;
    int yOffset = optionsOffset;
    int i;
    for (i = 0; i < optionsLength; i++)
    {
        drawStringCentered(
            options[i],
            halfWidth,
            yOffset,
            false);
        yOffset += step;
    }
}

Graphics_Rectangle _drawSelectionRectangle(const int selected, const int step)
{
    const int pad = 20;
    Graphics_Rectangle rectangle;
    rectangle.xMin = pad;
    rectangle.xMax = DISPLAY_WIDTH - pad;
    rectangle.yMin = optionsOffset + selected * step - step / 2;
    rectangle.yMax = optionsOffset + selected * step + step / 2;

    if (selected == optionsLength)
    {
        rectangle.xMin = DISPLAY_WIDTH - 20;
        rectangle.xMax = DISPLAY_WIDTH - 1;
        rectangle.yMin = DISPLAY_HEIGHT - 15;
        rectangle.yMax = DISPLAY_HEIGHT - 2;
    }
    drawRectangle(&rectangle);
    return rectangle;
}

void _cleanRectangle(const Graphics_Rectangle *rect)
{
    uint32_t fgColor = getForegroundColor();
    setForegroundColorTranslated(getBackgroundColor());
    drawRectangle(rect);
    setForegroundColorTranslated(fgColor);
}

void _drawLightDarkSwitch()
{
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
    drawString(
        str,
        DISPLAY_WIDTH - 12,
        DISPLAY_HEIGHT - getFontHeight() - 3,
        true);
}

void adjustBrightness(bool *previouslyDark)
{
    bool useDark = previouslyDark;
    switch (gameState.brightness)
    {
    case AUTO:
    { //Get luminosity
        unsigned long int lux = getLux();

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
            setBackgroundColor(0x00232323);
            setForegroundColor(GRAPHICS_COLOR_ANTIQUE_WHITE);
            *previouslyDark = true;
            clearDisplay();
        }
    }
    else
    {
        if (*previouslyDark)
        {
            setBackgroundColor(GRAPHICS_COLOR_WHITE);
            setForegroundColor(GRAPHICS_COLOR_BLACK);
            *previouslyDark = false;
            clearDisplay();
        }
    }
}
