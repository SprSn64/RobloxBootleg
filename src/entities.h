#ifndef ENTITIES_H
#define ENTITIES_H

#include <types.h>

//make all the objects use the same struct type thing with options for showing different properties and stuff

typedef struct PartClass{
	Vector3 pos, rot, scale;
	CharColour colour;
} PartClass

#endif