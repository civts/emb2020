#include "utils.h"
//Returns true if these rectangles overlap, false otherwise
bool isOverlapping(const Graphics_Rectangle *rect1, const Graphics_Rectangle *rect2)
{
    if (
        rect1->xMax >= rect2->xMin && rect1->xMax <= rect2->xMax &&
        rect1->yMax >= rect2->yMin && rect1->yMax <= rect2->yMax)
    {
        return true;
    }
    else if (
        rect1->xMin >= rect2->xMin && rect1->xMin <= rect2->xMax &&
        rect1->yMax >= rect2->yMin && rect1->yMax <= rect2->yMax)
    {
        return true;
    }
    else if (
        rect1->xMin >= rect2->xMin && rect1->xMin <= rect2->xMax &&
        rect1->yMin >= rect2->yMin && rect1->yMin <= rect2->yMax)
    {
        return true;
    }
    else if (
        rect1->xMax >= rect2->xMin && rect1->xMax <= rect2->xMax &&
        rect1->yMin >= rect2->yMin && rect1->yMin <= rect2->yMax)
    {
        return true;
    }
    return false;
}
