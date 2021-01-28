#include "menu.h"
#include <stdio.h>

void showMenu(Graphics_Context *gContext, gameInfo_t *game_info)
{
    Graphics_drawStringCentered(gContext,(int8_t *) "TEST", AUTO_STRING_LENGTH, 64, 30, true);
}

void cleanScreen(Graphics_Context *gContext)
{
    Graphics_clearDisplay(gContext);
}
