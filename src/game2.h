#include <stdbool.h>
#ifndef __GAME_2_H__
#define __GAME_2_H__

enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

//Let player choose maximum snake length
inline int chooseMaxLength();

//Play game 2.
//Returns true if won, false if lost
bool game2();

#endif
