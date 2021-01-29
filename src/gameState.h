#include <stdbool.h>
#include "ti/grlib/grlib.h"
#ifndef __GAME_INFO_H__
#define __GAME_INFO_H__

#define J_UP_TRESH 10000
#define J_DOWN_TRESH 7000
#define J_RIGHT_TRESH 10000
#define J_LEFT_TRESH 6000

enum screen
{
    Settings,
    Game1,
    Game2,
    Changing
};

typedef struct
{
    //The game the player is going to play
    int selectedGame;
    //Wether to use dark colors or not
    bool useDarkMode;
    //The screen the user is viewing
    enum screen screenIAmIn;
    //This allows access to the display graphical context
    Graphics_Context gContext;

    bool topButtonClicked;

    uint16_t joystickX;
    uint16_t joystickY;
} GameState_t;

GameState_t gameState;

#endif
