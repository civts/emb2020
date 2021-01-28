#include <stdbool.h>
#ifndef __GAME_INFO__
#define __GAME_INFO__
/*This structure holds the data for the game
*/
typedef struct
{
    //The game the player is going to play
    int selectedGame;
    //Wether to use dark colors or not
    bool useDarkMode;
} gameInfo_t;
#endif
