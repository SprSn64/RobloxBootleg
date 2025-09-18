#include "materials.h"
#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "math.h"
#include <types.h>

//create colors violently

CharColor* SetCharColor(U8 r, U8 g, U8 b, U8 a) {
	CharColor* localColor = malloc(sizeof(CharColor));
	localColor->r = r;
	localColor->g = g;
	localColor->b = b;
	localColor->a = a;
	return localColor;
}

Color* SetColor(F32 r, F32 g, F32 b) {
	Color* localColor = malloc(sizeof(Color));
	localColor->r = r;
	localColor->g = g;
	localColor->b = b;
	return localColor;
}