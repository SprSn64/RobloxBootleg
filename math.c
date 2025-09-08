#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "math.h"
#include "structs.h"

float lerp(float a, float b, float t){
	return(a + t * (b - a));
}
float invLerp(float a, float b, float v){
	return (a - v) / (b - a); 
}

float *multMatrix(float matrixA[16], float matrixB[16]){
	//horrible code warning
	float *output;
	output = malloc(sizeof(float) * 16);
	
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

Vector4 matrixTransform(Vector4 input, float matrix[16]){
	Vector4 output = {
		input.x * matrix[0] + input.y * matrix[4] + input.z * matrix[8] + input.w * matrix[12],
		input.x * matrix[1] + input.y * matrix[5] + input.z * matrix[9] + input.w * matrix[13],
		input.x * matrix[2] + input.y * matrix[6] + input.z * matrix[10] + input.w * matrix[14],
		input.x * matrix[3] + input.y * matrix[7] + input.z * matrix[11] + input.w * matrix[15],
	};
	return output;
}

float *initMatrix(){
	float *output;
	output = malloc(sizeof(float) * 16);
	for(int i=0; i<16; i++)output[i] = 0;
	output[0] = 1.f; output[5] = 1.f; output[10] = 1.f; output[15] = 1.f; 
	return output;
}

float *perspMatrix(float fov, float aspect, float zNear, float zFar){
	char rORl = 0;
	char ZOorNO = 1;
	float halfFov = tan(fov/2);
	float *output = initMatrix();
	
	output[0] = 1.f / (halfFov * aspect); 
	output[5] = 1.f / halfFov;
	output[11] = 1 - 2 * rORl; 
	if(!ZOorNO){
		output[10] = zFar / (zNear - zFar); 
		output[14] = -(zFar * zNear) / (zNear - zFar);
	}else{
		output[10] = -(zFar + zNear) / (zNear - zFar); 
		output[14] = -(2 * zFar * zNear) / (zNear - zFar);
	}
	
	return output;
}

int matrixRotate(float matrix[16], Vector3 rotate){
	float *newMatrix = initMatrix();
	float calcs[6] = {sin(rotate.x), cos(rotate.x), sin(rotate.y), cos(rotate.y), sin(rotate.z), cos(rotate.z)};
	
	float *xMatrix = initMatrix();
	xMatrix[5] = calcs[1]; xMatrix[6] = -calcs[0];
	xMatrix[9] = calcs[0]; xMatrix[10] = calcs[1];
	
	float *yMatrix = initMatrix();
	yMatrix[0] = calcs[3]; yMatrix[2] = calcs[2];
	yMatrix[8] = -calcs[2]; yMatrix[10] = calcs[3];
	
	float *zMatrix = initMatrix();
	zMatrix[0] = calcs[5]; zMatrix[1] = -calcs[4];
	zMatrix[4] = calcs[4]; zMatrix[5] = calcs[5];
	
	newMatrix = multMatrix(multMatrix(multMatrix(matrix, xMatrix), yMatrix), zMatrix);
	
	memcpy(matrix, newMatrix, sizeof(float) * 16);
	return 0;
}

int matrixTranslate(float matrix[16], Vector3 translate){
	matrix[12] += translate.x; matrix[13] += translate.y; matrix[14] += translate.z;
	return 0;
}