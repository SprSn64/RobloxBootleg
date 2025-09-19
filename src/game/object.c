#include "object.h"
#include "model.h"
#include <glad/gl.h>

Object objects[MAX_OBJECTS];
S32 objectCount = 0;

Object currentProcObj;

Object createObject(ObjBhv bhv, U16 model, Vector3 pos, Vector3 rot){
    Object obj;
    Graphics gfx = modelToGraphics[model];
    obj.verts = gfx.vtx;
    obj.faces = gfx.tri;
    obj.vertSize = gfx.vsize;
    obj.faceSize = gfx.tsize;
    obj.timer = 0;
    obj.pos = pos;
    obj.basePos = pos;
    obj.rot = rot;
    obj.scale = (Vector3){1,1,1};
    obj.bhv = bhv;

    glGenVertexArrays(1, &obj.VAO);
    glBindVertexArray(obj.VAO);

    glGenBuffers(1, &obj.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, gfx.vsize, gfx.vtx, GL_STATIC_DRAW);

    glGenBuffers(1, &obj.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gfx.tsize, gfx.tri, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (U0)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (U0)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    obj.indexCount = (S32)(gfx.tsize / sizeof(U32));

    objects[objectCount++] = obj;
    return obj;
}