#include "materials.h"
#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "math.h"
#include <types.h>

//create colors violently

CharColour* SetCharColor(U8 r, U8 g, U8 b, U8 a) {
	CharColour* localColor = malloc(sizeof(CharColour));
	localColor->r = r;
	localColor->g = g;
	localColor->b = b;
	localColor->a = a;
	return localColor;
}

Colour* SetColor(F32 r, F32 g, F32 b) {
	Colour* localColor = malloc(sizeof(Colour));
	localColor->r = r;
	localColor->g = g;
	localColor->b = b;
	return localColor;
}