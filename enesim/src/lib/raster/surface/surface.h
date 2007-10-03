#ifndef _SURFACE_H
#define _SURFACE_H

typedef struct _Enesim_Surface_Compositor_Func
{

} Enesim_Surface_Compositor_Func;

typedef struct _Enesim_Surface_Func
{

} Enesim_Surface_Func;

typedef struct _Argb8888_Data
{
	DATA32	*data;
} Argb8888_Data;

typedef struct _Rgb565_Data
{
	DATA16	*data;
	DATA8 	*alpha;
} Rgb565_Data;

struct _Enesim_Surface
{
	int 				w;
	int 				h;
	Enesim_Surface_Format		format;
	int 				flags;
	union
	{
		Rgb565_Data 	rgb565;	
		Argb8888_Data 	argb8888;	
	} data;
	Enesim_Surface_Func 		*funcs;
	Enesim_Surface_Compositor_Func 	cfuncs;
};

#endif
