#ifndef MODEL_h
#define MODEL_h

#include <types.h>

typedef struct Graphics {
    F32 *vtx; U32 *tri;
    U32 vsize, tsize;
} Graphics;


enum ModelID { 
    MODEL_CUBE,
    MODEL_PYRAMID
};

extern Graphics modelToGraphics[];
#endif // MODEL_h