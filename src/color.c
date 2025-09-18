#include "color.h"
#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "math.h"
#include <types.h>

//create colors violently

CharColour* SetCharColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    CharColour* localColor = malloc(sizeof(CharColour));
    localColor->r = r;
    localColor->g = g;
    localColor->b = b;
    localColor->a = a;
    return localColor;
}

Colour* SetColor(float r, float g, float b) {
    Colour* localColor = malloc(sizeof(Colour));
    localColor->r = r;
    localColor->g = g;
    localColor->b = b;
    return localColor;
}