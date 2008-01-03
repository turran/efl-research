#ifndef PRIVATE_H_
#define PRIVATE_H_

struct _Eina_Object
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

struct _Eina_Canvas
{
	int w;
	int h;
	/* possible callbacks */
	// rectangle_push: in case of double buffer, this will inform what has changed (the last rendered rectangle)
	// what about post processing of the canvas??
};


#endif /*PRIVATE_H_*/
