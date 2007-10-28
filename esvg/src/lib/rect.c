#include "ESVG.h"
#include "esvg_private.h"

typedef struct _ESVG_Rect
{
	ESVG_Shape shape;
	float x;
	float y;
	float w;
	float h;
	float rx;
	float ry;
} Esvg_Rect;