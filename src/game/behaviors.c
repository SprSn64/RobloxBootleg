#include "behaviors.h"
#include <math.h>
#include <stdio.h>

void cubeLoop(Object *o)
{
    o->pos.y = o->basePos.y + sinf(o->timer * 300.0f) * 0.7f;
}

ObjBhv bhv_Pyramid = {
    NULL,
    NULL,
};

ObjBhv bhv_Static_Cube = {
    NULL,
    cubeLoop,
};