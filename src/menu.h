#include "gameInfo.h"
#include "ti/grlib/grlib.h"
#ifndef __MENU__
#define __MENU__
//Show menu where player can select level to play and
//desired color theme (light, dark or automatic)
void showMenu(Graphics_Context *gContext, gameInfo_t *game_info);
#endif
