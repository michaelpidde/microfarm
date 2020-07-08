#pragma once

#include "types.h"

DragContainer _create_container(uint32 x, uint32 y, uint32 w, uint32 h, char *id);
Rect get_drag_bar_position(DragContainer *container);