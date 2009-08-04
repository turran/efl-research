#include "eranoid.h"

typedef struct _Bar
{
	Eon_Rect *shape;
} Bar;


Bar * bar_new(Eon_Canvas *c)
{
	Bar *b;
	Eon_Rect *s;

	b = malloc(sizeof(Ball));
	s = eon_rect_new(c);
	
	eon_rect_x_set(s, 320/2 - 20);
	eon_rect_y_set(s, 240 - 5);
	eon_rect_w_set(s, 40);
	eon_rect_h_set(s, 10);
	eon_rect_color_set(s, 0xff000000);
	eon_rect_show(s);

	b->shape = s;
}


