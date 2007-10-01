#ifndef _SCANLINE_H
#define _SCANLINE_H

typedef void *(*Enesim_Scanline_Create)(void);
typedef void (*Enesim_Scanline_Add)(void *d, int x0, int x1, int y, int coverage);
typedef void (*Enesim_Scanline_Free)(void *d);
#define ETC_SCANLINE_CREATE(function) 	((Enesim_Scanline_Create(function))
#define	ETC_SCANLINE_FREE(function) 	((Enesim_Scanline_Free(function))
#define	ETC_SCANLINE_ADD(function) 	((Enesim_Scanline_Add(function))

typedef struct _Enesim_Scanline_Func
{
	Enesim_Scanline_Create 	create;
	Enesim_Scanline_Add 	add;
	Enesim_Scanline_Free 	free;
} Enesim_Scanline_Func;

struct _Enesim_Scanline
{
	Enesim_Scanline_Func 	*funcs;
	void 			*data;
};

extern Enesim_Scanline_Func naa;

#endif
