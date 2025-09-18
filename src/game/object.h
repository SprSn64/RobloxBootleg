#ifndef OBJECT_h
#define OBJECT_h

#include <types.h>

typedef struct Object Object; // forward

typedef struct {
    void (*init)(void);
    void (*update)(Object *o);
} ObjBhv;

typedef struct Object {
    F32 *verts;
    U32 *faces;
    U32 vertSize;
    U32 faceSize;
    U32 VAO, VBO, EBO;
    S32 indexCount;

    ObjBhv bhv;
    Vector3 pos;
    Vector3 basePos;
    Vector3 rot;
    Vector3 scale;
    F32 timer;
} Object;


#define MAX_OBJECTS 32
extern S32 objectCount;
extern Object objects[MAX_OBJECTS];

extern Object currentProcObj;

extern Object createObject(ObjBhv bhv, U16 model, Vector3 pos, Vector3 rot, Vector3 scale);
#endif //OBJECT_h