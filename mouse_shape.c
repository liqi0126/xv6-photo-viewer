#include "types.h"
#include "defs.h"
#include "gui_base.h"
#include "mouse_shape.h"

const uchar mouse_pointer[MOUSE_MODE][MOUSE_HEIGHT][MOUSE_WIDTH] = {
    {
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
        {2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0},
        {2, 1, 1, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0},
        {2, 1, 1, 2, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0},
        {2, 1, 2, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0}
    }, {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 1, 0},
        {1, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
        {1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}
    }
};