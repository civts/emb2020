#include <stdbool.h>
#ifndef __GAME_INFO__
#define __GAME_INFO__
/*This structure holds the data for the game
*/
typedef struct
{
    //The level the player is going to play
    int selectedLevel;
    //Wether to use dark colors or not
    bool useDarkMode;
} gameInfo_t;
#endif
