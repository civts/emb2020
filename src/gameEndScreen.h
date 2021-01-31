#include <stdbool.h>

#ifndef __GAME_END_H__
#define __GAME_END_H__

//Displays won or lost message,
//plays win/loss tone,
//then goes to LPM_0 until button is pressed
void gameEndScreen(bool won);

#endif
