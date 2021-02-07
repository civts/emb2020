#include "hw_dependent/pcm.h"
#include "hw_dependent/hwInit.h"
#include "game1.h"
#include "game2.h"
#include "gameState.h"
#include "gameEndScreen.h"
#include "menu.h"

int main(void)
{
  gameState.buttonClicked = false;
  gameState.joystickX = J_H_INITIAL;
  gameState.joystickY = J_V_INITIAL;
  gameState.brightness = AUTO;

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
    case 1:
      won = game2();
      break;
    default:
      won = false;
      break;
    }

    //Display won or lost message,
    gameEndScreen(won);

    //Go to low power mode until button is pressed
    gotoLPM();
  }
}
