#include "model.h"

F32 triVerts[] = {
    -0.5f,-0.5f,-0.5f, 1.f,0.f,0.f,
     0.5f,-0.5f,-0.5f, 0.f,1.f,0.f,
     0.5f,-0.5f, 0.5f, 0.f,0.f,1.f,
    -0.5f,-0.5f, 0.5f, 1.f,1.f,0.f,
     0.f, 0.5f, 0.f, 1.f,0.f,1.f
};

U32 triFaces[] = {
    0,1,4, 1,2,4, 2,3,4, 3,0,4, 2,1,0, 0,3,2
};

F32 cubeVerts[] = {
    -0.5f,-0.5f,-0.5f, 1,0,0,
     0.5f,-0.5f,-0.5f, 0,1,0,
     0.5f, 0.5f,-0.5f, 0,0,1,
    -0.5f, 0.5f,-0.5f, 1,1,0,
    -0.5f,-0.5f, 0.5f, 1,0,1,
     0.5f,-0.5f, 0.5f, 0,1,1,
     0.5f, 0.5f, 0.5f, 1,1,1,
    -0.5f, 0.5f, 0.5f, 0,0,0,
};
U32 cubeFaces[] = {
    0,1,2, 0,2,3,
    1,5,6, 1,6,2,
    5,4,7, 5,7,6,
    4,0,3, 4,3,7,
    3,2,6, 3,6,7,
    4,5,1, 4,1,0
};

#define VTX_TRI(v, t) {v, t, sizeof(v), sizeof(t)}
Graphics modelToGraphics[] = {
    VTX_TRI(cubeVerts, cubeFaces),
    VTX_TRI(triVerts, triFaces)
};