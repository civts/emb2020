#include "src/hwInit.h"
#include "src/gameInfo.h"
#include "src/menu.h"

int main(void)
{
  gameInfo.screenIAmIn = Settings;
  gameInfo.useDarkMode = false;

  //Init hardware and graphics
  hwInit();

  while (1)
  {
    gameInfo.selectedGame = 0;

    //Let player choose game and color theme
    showMenu();
    //Let player play the game
    //showGameScreen();

    //Display message "restart by pressing S1"
    // Go to low power mode
    PCM_gotoLPM0();
  }
}
