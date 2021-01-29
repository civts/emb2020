#include "src/hwInit.h"
#include "src/gameState.h"
#include "src/menu.h"

int main(void)
{
  gameState.screenIAmIn = Settings;
  gameState.useDarkMode = false;
  gameState.topButtonClicked = false;

  //Init hardware and graphics
  hwInit();

  while (1)
  {
    gameState.selectedGame = 0;

    //Let player choose game and color theme
    showMenu();
    //Let player play the game
    //showGameScreen();

    //Display message "restart by pressing S1"
    // Go to low power mode
    PCM_gotoLPM0();
  }
}
