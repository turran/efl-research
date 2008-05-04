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
