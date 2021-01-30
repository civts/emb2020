#ifndef __MENU_H__
#define __MENU_H__
//Show menu where player can select the game to play and
//desired color theme (light, dark or automatic)
void showMenu();

//Draws the title on the screen
void _drawTitle();

//Draws the options on the screen
void _drawOptions(int step);

//Draws the selection rectangle
//The selection rectangle indicates the area the user is interacting with
void _drawSelectionRectangle(const int selected, const int step);

//Deletes the selection rectangle by drawing over it another
//one of with the same color as the background
void _cleanSelectionRectangle(const int selected, const int step);

#endif
