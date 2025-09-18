#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "math.h"
#include <types.h>

F32 lerp(F32 a, F32 b, F32 t){
	return (a + t * (b - a));
}
F32 invLerp(F32 a, F32 b, F32 v){
	return (a - v) / (b - a);
}

F32 *multMatrix(MTX16 matrixA, MTX16 matrixB){
	// horrible code warning
	float *output;
	output = malloc(sizeof(MTX16));

	output[0] = matrixA[0] * matrixB[0] + matrixA[4] * matrixB[1] + matrixA[8] * matrixB[2] + matrixA[12] * matrixB[3];
	output[1] = matrixA[1] * matrixB[0] + matrixA[5] * matrixB[1] + matrixA[9] * matrixB[2] + matrixA[13] * matrixB[3];
	output[2] = matrixA[2] * matrixB[0] + matrixA[6] * matrixB[1] + matrixA[10] * matrixB[2] + matrixA[14] * matrixB[3];
	output[3] = matrixA[3] * matrixB[0] + matrixA[7] * matrixB[1] + matrixA[11] * matrixB[2] + matrixA[15] * matrixB[3];
	output[4] = matrixA[0] * matrixB[4] + matrixA[4] * matrixB[5] + matrixA[8] * matrixB[6] + matrixA[12] * matrixB[7];
	output[5] = matrixA[1] * matrixB[4] + matrixA[5] * matrixB[5] + matrixA[9] * matrixB[6] + matrixA[13] * matrixB[7];
	output[6] = matrixA[2] * matrixB[4] + matrixA[6] * matrixB[5] + matrixA[10] * matrixB[6] + matrixA[14] * matrixB[7];
	output[7] = matrixA[3] * matrixB[4] + matrixA[7] * matrixB[5] + matrixA[11] * matrixB[6] + matrixA[15] * matrixB[7];
	output[8] = matrixA[0] * matrixB[8] + matrixA[4] * matrixB[9] + matrixA[8] * matrixB[10] + matrixA[12] * matrixB[11];
	output[9] = matrixA[1] * matrixB[8] + matrixA[5] * matrixB[9] + matrixA[9] * matrixB[10] + matrixA[13] * matrixB[11];
	output[10] = matrixA[2] * matrixB[8] + matrixA[6] * matrixB[9] + matrixA[10] * matrixB[10] + matrixA[14] * matrixB[11];
	output[11] = matrixA[3] * matrixB[8] + matrixA[7] * matrixB[9] + matrixA[11] * matrixB[10] + matrixA[15] * matrixB[11];
	output[12] = matrixA[0] * matrixB[12] + matrixA[4] * matrixB[13] + matrixA[8] * matrixB[14] + matrixA[12] * matrixB[15];
	output[13] = matrixA[1] * matrixB[12] + matrixA[5] * matrixB[13] + matrixA[9] * matrixB[14] + matrixA[13] * matrixB[15];
	output[14] = matrixA[2] * matrixB[12] + matrixA[6] * matrixB[13] + matrixA[10] * matrixB[14] + matrixA[14] * matrixB[15];
	output[15] = matrixA[3] * matrixB[12] + matrixA[7] * matrixB[13] + matrixA[11] * matrixB[14] + matrixA[15] * matrixB[15];

	return output;
}

Vector4 matrixTransform(Vector4 input, MTX16 matrix){
	Vector4 output = {
		input.x * matrix[0] + input.y * matrix[4] + input.z * matrix[8] + input.w * matrix[12],
		input.x * matrix[1] + input.y * matrix[5] + input.z * matrix[9] + input.w * matrix[13],
		input.x * matrix[2] + input.y * matrix[6] + input.z * matrix[10] + input.w * matrix[14],
		input.x * matrix[3] + input.y * matrix[7] + input.z * matrix[11] + input.w * matrix[15],
	};
	return output;
}

F32 *initMatrix(){
	F32 *output;
	output = malloc(sizeof(MTX16));
	for (int i = 0; i < 16; i++)
		output[i] = 0;
	output[0] = 1.f;
	output[5] = 1.f;
	output[10] = 1.f;
	output[15] = 1.f;
	return output;
}

F32 *perspMatrix(F32 fov, F32 aspect, F32 zNear, F32 zFar){
	char rORl = 0;
	char ZOorNO = 1;
	F32 halfFov = tan(fov / 2);
	F32 *output = initMatrix();

	output[0] = 1.f / (halfFov * aspect);
	output[5] = 1.f / halfFov;
	output[11] = 1 - 2 * rORl;
	if (!ZOorNO)
	{
		output[10] = zFar / (zNear - zFar);
		output[14] = -(zFar * zNear) / (zNear - zFar);
	}
	else
	{
		output[10] = -(zFar + zNear) / (zNear - zFar);
		output[14] = -(2 * zFar * zNear) / (zNear - zFar);
	}

	return output;
}

S32 matrixRotate(MTX16 matrix, Vector3 rotate){
	F32 *newMatrix = initMatrix();
	F32 calcs[6] = {sin(rotate.x), cos(rotate.x), sin(rotate.y), cos(rotate.y), sin(rotate.z), cos(rotate.z)};

	F32 *xMatrix = initMatrix();
	xMatrix[5] = calcs[1];
	xMatrix[6] = -calcs[0];
	xMatrix[9] = calcs[0];
	xMatrix[10] = calcs[1];

	F32 *yMatrix = initMatrix();
	yMatrix[0] = calcs[3];
	yMatrix[2] = calcs[2];
	yMatrix[8] = -calcs[2];
	yMatrix[10] = calcs[3];

	F32 *zMatrix = initMatrix();
	zMatrix[0] = calcs[5];
	zMatrix[1] = -calcs[4];
	zMatrix[4] = calcs[4];
	zMatrix[5] = calcs[5];

	newMatrix = multMatrix(multMatrix(multMatrix(matrix, xMatrix), yMatrix), zMatrix);

	free(xMatrix);
	free(yMatrix);
	free(zMatrix);
	
	memcpy(matrix, newMatrix, sizeof(MTX16));
	return 0;
}

S32 matrixTranslate(MTX16 matrix, Vector3 translate){
	matrix[12] += translate.x;
	matrix[13] += translate.y;
	matrix[14] += translate.z;
	return 0;
}

S32 matrixScale(MTX16 matrix, Vector3 scale){
	matrix[0] *= scale.x;
	matrix[5] *= scale.y;
	matrix[11] *= scale.z;
	return 0;
}