#include "pause_screen.h"
#include "gameState.h"
#include "hw_dependent/display.h"
#include "hw_dependent/pcm.h"

void pauseScreen()
{
    //Enter pause
    clearDisplay();
    gameState.buttonClicked = false;
    while (!gameState.buttonClicked)
    {
        //Draw pause screen
        drawStringCentered(
            "PAUSE",
            LCD_HORIZONTAL_MAX / 2,
            20,
            false);
        drawStringCentered(
            "Press S1 to resume",
            LCD_HORIZONTAL_MAX / 2,
            80,
            false);
        //Wait for button
        gotoLPM();
    }
    gameState.buttonClicked = false;
    clearDisplay();
}
