#ifndef _SURFACE_H
#define _SURFACE_H

typedef struct _Enesim_Surface_Compositor_Func
{

} Enesim_Surface_Compositor_Func;

typedef struct _Enesim_Surface_Func
{

} Enesim_Surface_Func;

struct _Enesim_Surface
{
	int 				w;
	int 				h;
	int 				format;
	void 				*data;
	Enesim_Surface_Func 		*funcs;
	Enesim_Surface_Compositor_Func 	cfuncs;
};

#endif
