#ifndef _ETC_PRIVATE_H
#define _ETC_PRIVATE_H

typedef struct _Etc_Rasterizer
{
	void (*vertex_add)(Etc_Scanline *s, float x, float y);
	void (*generate)(Etc_Scanline *s);
} Etc_Rasterizer;


#endif

