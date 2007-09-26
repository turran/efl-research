#ifndef _RASTERIZER_H
#define _RASTERIZER_H

typedef struct _Etc_Rasterizer_Func
{
	void *(*create)(void);
	void (*vertex_add)(void *r, float x, float y);
	void (*generate)(void *r, Etc_Scanline *sl);
} Etc_Rasterizer_Func;

extern Etc_Rasterizer_Func gcp;
extern Etc_Rasterizer_Func faap;

struct _Etc_Rasterizer
{
	Etc_Rasterizer_Func 	*funcs;
	void 			*data;
};



#endif
