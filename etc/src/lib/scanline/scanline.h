#ifndef _SCANLINE_H
#define _SCANLINE_H

typedef void *(*Etc_Scanline_Create)(void);
typedef void (*Etc_Scanline_Add)(void *d, int x0, int x1, int y, int coverage);
typedef void (*Etc_Scanline_Free)(void *d);
#define ETC_SCANLINE_CREATE(function) 	((Etc_Scanline_Create(function))
#define	ETC_SCANLINE_FREE(function) 	((Etc_Scanline_Free(function))
#define	ETC_SCANLINE_ADD(function) 	((Etc_Scanline_Add(function))

typedef struct _Etc_Scanline_Func
{
	Etc_Scanline_Create 	create;
	Etc_Scanline_Add 	add;
	Etc_Scanline_Free 	free;
} Etc_Scanline_Func;

struct _Etc_Scanline
{
	Etc_Scanline_Func 	*funcs;
	void 			*data;
};

extern Etc_Scanline_Func naa;

#endif
