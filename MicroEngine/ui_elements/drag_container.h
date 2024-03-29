#pragma once

#include "types.h"

DragContainer _create_container(Rect position, char *id);
Rect get_drag_bar_position(DragContainer *container);
Rect get_close_button_position(DragContainer *container);