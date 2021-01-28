#include "gameInfo.h"
#include "ti/grlib/grlib.h"
#ifndef __MENU__
#define __MENU__
//Show menu where player can select the game to play and
//desired color theme (light, dark or automatic)
void showMenu(Graphics_Context *context, gameInfo_t *game_info);

//Draws the title on the screen
void _drawTitle(Graphics_Context *context);

//Draws the options on the screen
void _drawOptions(Graphics_Context *context, int step);

//Draws the selection rectangle
//The selection rectangle indicates the area the user is interacting with
void _drawSelectionRectangle(Graphics_Context *context, const int selected, const int step);

#endif
