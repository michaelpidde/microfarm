#pragma once

#include "../Common/types.h"


/*******************************************************************************
 * Determine whether there is collision between two rectangles.
 * 
 * INPUT:
 * Rect -- First rectangle
 * Rect -- Second rectangle
 * 
 * OUTPUT:
 * int  -- Boolean result
 ******************************************************************************/
int MCR_rect_overlap(Rect r1, Rect r2) {
    if(r1.x <= r2.x + r2.w &&
        r1.x + r1.w >= r2.x &&
        r1.y <= r2.y + r2.h &&
        r1.y + r1.h >= r2.y)
    {
        return 1;
    }
    
    return 0;
}