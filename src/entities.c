#include "entities.h"
#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "math.h"
#include <types.h>
#include "color.h"

//kill people violently
PartClass* TestPart;

PartClass* CreatePartClass(Vector3 pos, Vector3 rot, Vector3 scale, CharColour colour) {
    PartClass* localPartClass = malloc(sizeof(PartClass));
    localPartClass->pos = pos;
    localPartClass->rot = rot;
    localPartClass->scale = scale;
    localPartClass->colour = colour;
    printf("Created Part!\n");
    return localPartClass;
}

void DeletePartClass(PartClass* dPart) {
    free(dPart);
    printf("deleted Part!\n");
}