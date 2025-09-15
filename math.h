#ifndef MATH_H
#define MATH_H

#include "structs.h"

#define PI 3.141259
#define DEG2RAD PI / 180
#define RAD2DEG 180 / PI

float lerp(float a, float b, float t);
float invLerp(float a, float b, float v);

Vector4 matrixTransform(Vector4 input, float matrix[16]);
float *initMatrix();

float *perspMatrix(float fov, float aspect, float zNear, float zFar);

float *multMatrix(float matrixA[16], float matrixB[16]);
int matrixTranslate(float matrix[16], Vector3 translate);
int matrixRotate(float matrix[16], Vector3 rotate);
int matrixScale(float matrix[16], Vector3 scale);

#endif