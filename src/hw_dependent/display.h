#include <stdbool.h>
#include "../../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define DISPLAY_WIDTH LCD_HORIZONTAL_MAX
#define DISPLAY_HEIGHT LCD_VERTICAL_MAX

#define titleFont &g_sFontCm16b

//Clear the display by filling it with background color
void clearDisplay();

//Returns foreground color currently being used
uint32_t getForegroundColor();

//Returns background color currently being used
uint32_t getBackgroundColor();

//Returns font height currently being used
uint16_t getFontHeight();

//Returns font currently being used
Graphics_Font *getFont();

//Sets font to be used
void setFont(const Graphics_Font *font);

//Sets foreground color
//note: color must already have been translated for this display
void setForegroundColorTranslated(uint32_t color);

//Sets background color
//note: color must already have been translated for this display
void setBackgroundColorTranslated(uint32_t color);

//Translates the color to the one that is right for the display,
//then sets it as foreground
void setForegroundColor(uint32_t color);

//Translates the color to the one that is right for the display,
//then sets it as background
void setBackgroundColor(uint32_t color);

//Paints the rectanlge on the screen (just the border)
void drawRectangle(const Graphics_Rectangle *rectangle);

//Paints the rectanlge on the screen (filled with fg color)
void fillRectangle(const Graphics_Rectangle *rectangle);

//Draws a filled circle on the screen
void fillCircle(int x, int y, int radius);

//Draws a string on the display, starting from the given point
void drawString(const char *string, uint32_t x, uint32_t y, bool redrawBackground);

//Draws a string on the display, centered on the given point
void drawStringCentered(const char *string, uint32_t x, uint32_t y, bool redrawBackground);

//Detects if two rectangles overlap each other
bool isOverlapping(const Graphics_Rectangle *rect1, const Graphics_Rectangle *rect2);

#endif
