#pragma once

#include "micro_engine.h"

SDL_Rect to_SDL_Rect(Rect r) {
    SDL_Rect rect;
    rect.x = r.x;
    rect.y = r.y;
    rect.w = r.w;
    rect.h = r.h;
    return rect;
}