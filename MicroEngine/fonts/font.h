#pragma once

#include "..\..\Common\types.h"

#define GLYPH_COUNT 95

typedef struct Glyph {
    uint32 id;
    uint32 x;
    uint32 y;
    uint32 w;
    uint32 h;
    int xoffset;
    int yoffset;
    uint32 xadvance;
} Glyph;