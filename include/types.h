#ifndef STRUCTS_H
#define STRUCTS_H

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

typedef void *U0;
typedef unsigned char    U8;
typedef signed char      S8;
typedef unsigned short   U16;
typedef signed short     S16;
typedef unsigned int     U32;
typedef signed int       S32;
typedef unsigned long long U64;
typedef long long        S64;
typedef float            F32;
typedef double           F64;

typedef F32 MTX16[16];

#undef NULL
#define NULL ((U0)0)

typedef struct KeyMap{
	char down, pressed;
	S32 scanCode;
} KeyMap;

typedef struct Vector2{
	F32 x, y;
} Vector2;

typedef struct Vector3{
	F32 x, y, z;
} Vector3;

typedef struct Vector4{
	F32 x, y, z, w;
} Vector4;

typedef struct CharColour{
	U8 r, g, b, a;
} CharColour;

typedef struct Colour{
	F32 r, g, b;
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