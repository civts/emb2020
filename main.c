#include <stdlib.h>
#include <stdio.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "src/hwInit.h"
#include "src/gameInfo.h"
#include "src/menu.h"

int main(void)
{
  Graphics_Context gContext;
  //Graphics_Display *g_sCrystalfontzDisplay = &g_sCrystalfontz128x128;
  gameInfo_t game_info;

  //Init hardware and graphics
  hwInit(&gContext);

  while (1)
  {
    game_info.selectedLevel = 0;
    game_info.useDarkMode = false;

    //Let player choose level and color theme
    showMenu(&gContext, &game_info);
    //Let player play the level
    //showGameScreen();

    //Display message "restart by pressing S1"
    // Go to low power mode
    PCM_gotoLPM0();
  }
}
