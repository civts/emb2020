#include "gameState.h"
#include "gameEndScreen.h"
#include "hw_dependent/buzzer.h"
#include "hw_dependent/display.h"

void gameEndScreen(bool won)
{
    clearDisplay();
 
    drawStringCentered(
        (won ? "YOU WON!" : "You lost"),
        DISPLAY_WIDTH / 2,
        20,
        false);

    won ? playWinTone() : playLossTone();

    drawStringCentered(
        "Press S1 to continue",
        DISPLAY_WIDTH / 2,
        80,
        false);
}
