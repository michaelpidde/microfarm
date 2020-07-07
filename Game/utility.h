#pragma once

#include "types.h"

/**
 * Get 2D grid movement increment or decrement based on direction.
 * 
 * INPUT:
 * Direction -- direction we want to convert
 * 
 * OUTPUT:
 * int       -- Signed unit of increment
 */
int direction_to_increment(Direction direction)
{
    switch(direction) {
        case North: {
            return -1;
        } break;
        case East: {
            return 1;
        } break;
        case South: {
            return 1;
        } break;
        case West: {
            return -1;
        }
    }
    return 0;
}