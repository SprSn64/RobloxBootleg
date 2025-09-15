#ifndef STRUCTS_H
#define STRUCTS_H

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

typedef struct KeyMap{
	char down, pressed;
	int scanCode;
} KeyMap;

typedef struct Vector2{
	float x, y;
} Vector2;

typedef struct Vector3{
	float x, y, z;
} Vector3;

typedef struct Vector4{
	float x, y, z, w;
} Vector4;

typedef struct CharColour{
	unsigned char r, g, b, a;
} CharColour;

typedef struct Colour{
	float r, g, b;
} Colour;

typedef struct Vertex{
	Vector2 pos;
	Colour colour;
} Vertex;

typedef struct Camera{
	Vector3 pos;
	Vector3 rot;
} Camera;

#endif