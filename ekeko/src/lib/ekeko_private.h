#ifndef EKEKO_PRIVATE_H_
#define EKEKO_PRIVATE_H_

/* 
 * Use cases:
 * 1. Filter Objects:
 * The filter object should receive the rectangles where to draw
 * get the pixels from the canvas surface and then filter them on its own surface for later bliting
 * or directly on the target surface.
 * 
 * 2. Double Buffered Canvas:
 * The canvas should have a surface attached i.e the back buffer and another surface on client side
 * to receive areas changed after the process of all objects on the canvas, i.e keep the areas that
 * changed buffered and then blit them from the backbuffer to the target surface. 
 * 
 * 3. Mirror objects, i.e objects that resembles other objects
 * There might be some kind of callback definitions whenever an object changes, something like object
 * listeners.
 * 
 */

/*
 * Tiler class functions
 * tile_size_set
 * rect_del
 * rect_add
 * clear
 * rects_get
 * rects_free
 * new
 * free 
 */

typedef struct _Ekeko_Tiler
{
	
} Ekeko_Tiler;

/* every object should receive the area or areas to draw based on their
 * boundings
 */

struct _Ekeko_Object
{
	int x;
	int y;
	int w;
	int h;
	
	/* possible callbacks */
	// pre?
	// post?
	// generate: make the object generate itself
};

struct _Ekeko_Canvas
{
	int w;
	int h;
	/* possible callbacks */
	// rectangle_push: in case of double buffer, this will inform what has changed (the last rendered rectangle)
	// what about post processing of the canvas??
	Ekeko_Tiler t;
};


#endif /*EKEKO_PRIVATE_H_*/
