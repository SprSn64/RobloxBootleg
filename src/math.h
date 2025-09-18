#ifndef MATH_H
#define MATH_H

#include <types.h>

#define PI 3.141259
#define DEG2RAD PI / 180
#define RAD2DEG 180 / PI

F32 lerp(F32 a, F32 b, F32 t);
F32 invLerp(F32 a, F32 b, F32 v);

Vector4 matrixTransform(Vector4 input, MTX16 matrix);
F32 *initMatrix();

F32 *perspMatrix(F32 fov, F32 aspect, F32 zNear, F32 zFar);

F32 *multMatrix(MTX16 matrixA, MTX16 matrixB);
S32 matrixTranslate(MTX16 matrix, Vector3 translate);
S32 matrixRotate(MTX16 matrix, Vector3 rotate);
S32 matrixScale(MTX16 matrix, Vector3 scale);

#endif