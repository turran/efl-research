#ifndef EKEKO_CANVAS_H_
#define EKEKO_CANVAS_H_

struct _Ekeko_Canvas
{
	int w;
	int h;
	/* possible callbacks */
	// rectangle_push: in case of double buffer, this will inform what has changed (the last rendered rectangle)
	// what about post processing of the canvas??
	Ekeko_Tiler *tiler;
	Ekeko_Object *objects;
	void *data;
};

#endif
