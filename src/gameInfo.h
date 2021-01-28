#include <stdbool.h>
#include "ti/grlib/grlib.h"
#ifndef __GAME_INFO__
#define __GAME_INFO__
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
    enum screen screenIAmIn;
    Graphics_Context gContext;
} gameInfo_t;

gameInfo_t gameInfo;

#endif
