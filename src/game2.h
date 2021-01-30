#include <stdbool.h>
#ifndef __GAME_2_H__
#define __GAME_2_H__

#define MAX_SNAKE_LEN 10

enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

//Play game 2.
//Returns true if won, false if lost
bool game2();

#endif
