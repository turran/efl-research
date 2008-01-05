#ifndef EKEKO_PRIVATE_H_
#define EKEKO_PRIVATE_H_

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
};


#endif /*EKEKO_PRIVATE_H_*/
