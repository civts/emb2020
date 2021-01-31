#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include "buzzer.h"
#include "gameState.h"
#include "gameEndScreen.h"

void gameEndScreen(bool won)
{
    Graphics_Context ctx = gameState.gContext;
    Graphics_clearDisplay(&ctx);

    Graphics_drawStringCentered(
        &ctx,
        (int8_t *)(won ? "YOU WON!" : "You lost"),
        AUTO_STRING_LENGTH,
        LCD_HORIZONTAL_MAX / 2,
        20,
        false);
    won ? playWinTone() : playLossTone();

    Graphics_drawStringCentered(
        &ctx,
        (int8_t *)"Press S1 to continue",
        AUTO_STRING_LENGTH,
        LCD_HORIZONTAL_MAX / 2,
        80,
        false);

    __sleep();
}
