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
    bool won;
    switch (gameState.selectedGame)
    {
    case 0:
      won = game1();
      break;

    default:
      won = false;
      break;
    }

    //Display won or lost message,
    //then go to LPM_0 until button is pressed
    gameEndScreen(won);
  }
}
