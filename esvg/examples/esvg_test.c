#include <stdio.h>
#include <stdlib.h>

#include "Eina.h"
#include "Enesim.h"
#include "ESVG.h"

#include "config.h"

#define WIDTH  640
#define HEIGHT 480

#if HAVE_CAIRO

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

#endif

/* Cairo engine */
#if HAVE_CAIRO

Display *dpy;

void * cairo_engine_create(void)
{
	
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	cairo_surface_t *cs;

	if(!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr = DefaultScreen(dpy);
	rootwin = RootWindow(dpy, scr);

	win=XCreateSimpleWindow(dpy, rootwin, 1, 1, WIDTH, HEIGHT, 0, 
			BlackPixel(dpy, scr), BlackPixel(dpy, scr));
	XStoreName(dpy, win, "esvg_test");
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask);
	XMapWindow(dpy, win);

	cs = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), WIDTH, HEIGHT);
	return cs;
}
#endif

int main(int argc, char **argv)
{
#if 0
	void *engine_data;
	ESVG_Engine_Type engine_type;
	ESVG *canvas;
	ESVG_Rect *rect, *background;
	ESVG_Shape *shape;
	ESVG_Length length;
	
#if HAVE_CAIRO
	engine_data = cairo_engine_create();
	engine_type = ESVG_ENGINE_CAIRO;
#endif
	length.type = ESVG_LENGTH_TYPE_NUMBER;
	
	canvas = esvg_svg_new(WIDTH, HEIGHT, engine_type, engine_data);
	background = esvg_rect_add(canvas);
#if 0
	esvg_rect_geometry_set(background, 0, 0, WIDTH, HEIGHT);
#endif
	shape = esvg_rect_shape_get(background);
	esvg_shape_fill_set(shape, 0xffffff);
	
	rect = esvg_rect_add(canvas);
#if 0
	esvg_rect_geometry_set(rect, 10, 20, 140, 280);
	esvg_rect_rounded_set(rect, 50, 15);
#endif
	shape = esvg_rect_shape_get(rect);
	esvg_shape_fill_set(shape, 0xf10b14);
	esvg_shape_stroke_set(shape, 0x000000);
	esvg_shape_opacity_set(shape, 1);
	while (1)
	{
#if HAVE_CAIRO
		XEvent e;
		XNextEvent(dpy, &e);

		if (e.type == Expose)
			esvg_damage_add(canvas, e.xexpose.x, e.xexpose.y, e.xexpose.width, e.xexpose.height);
#endif
		esvg_render(canvas);
	}
#endif
	return 0;
}
