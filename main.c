#include "src/game1.h"
#include "src/hwInit.h"
#include "src/gameState.h"
#include "src/menu.h"

int main(void)
{
  gameState.screenIAmIn = Changing;
  gameState.useDarkMode = false;
  gameState.topButtonClicked = false;
  gameState.joystickX = 8000;
  gameState.joystickY = 8000;

  //Init hardware and graphics
  hwInit();

  while (1)
  {
    gameState.selectedGame = 0;

    //Let player choose game and color theme
    showMenu();
    //Let player play the game
    switch (gameState.selectedGame)
    {
    case 0:
      game1();
      break;

    default:
      break;
    }

    //Display message "restart by pressing S1"
    // Go to low power mode
    PCM_gotoLPM0();
  }
}
