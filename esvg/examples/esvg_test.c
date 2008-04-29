#include <stdio.h>
#include <stdlib.h>

#include "Eina.h"
#include "Enesim.h"
#include "ESVG.h"

#include "config.h"

#define WIDTH  320
#define HEIGHT 240


#if HAVE_CAIRO

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

#endif

/* Cairo engine */
#if HAVE_CAIRO
void * cairo_engine_create(void)
{
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	cairo_surface_t *cs;

	if(!(dpy=XOpenDisplay(NULL))) {
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
	void *engine_data;
	ESVG_Engine_Type engine_type;
	ESVG *canvas;
	ESVG_Rect *rect;
	
#if HAVE_CAIRO
	engine_data = cairo_engine_create();
	engine_type = ESVG_ENGINE_CAIRO;
#endif
	canvas = esvg_new(WIDTH, HEIGHT, engine_type, engine_data);
	rect = esvg_rect_add(canvas);
	esvg_rect_geometry_set(rect, 0, 0, 140, 280);
	while (1)
	{
		esvg_render(canvas);
	}
	return 0;
}
