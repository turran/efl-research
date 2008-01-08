#ifndef EKEKO_TILER_H_
#define EKEKO_TILER_H_

typedef struct _Ekeko_Rectangle Ekeko_Rectangle;
typedef struct _Ekeko_Tiler Ekeko_Tiler;
typedef struct _Ekeko_Tiler_Class Ekeko_Tiler_Class;

struct _Ekeko_Rectangle
{
	Eina_Inlist list;
	Eina_Rectangle r;
};

struct _Ekeko_Tiler
{
	struct
	{
		int w;
		int h;
	} canvas;
	struct
	{
		int w;
		int h;
	} tile;
	void *data;
	Ekeko_Tiler_Class *class;
};

struct _Ekeko_Tiler_Class
{
	void * (*new)(int w, int h);
	void (*free)(Ekeko_Tiler *t);
	void (*tile_size_set)(Ekeko_Tiler *t, int w, int h);
	int (*rect_add)(Ekeko_Tiler *t, Eina_Rectangle *r);
	int (*rect_del)(Ekeko_Tiler *t, Eina_Rectangle *r);
	void (*clear)(Ekeko_Tiler *t);
	Ekeko_Rectangle * (*rects_get)(Ekeko_Tiler *t);
};

Ekeko_Tiler *ekeko_tiler_new(int type, int w, int h);
void ekeko_tiler_free(Ekeko_Tiler *t);
void ekeko_tiler_tile_size_set(Ekeko_Tiler *t, int w, int h);
int ekeko_tiler_rect_add(Ekeko_Tiler *t, Eina_Rectangle *r);
int ekeko_tiler_rect_del(Ekeko_Tiler *t, Eina_Rectangle *r);
void ekeko_tiler_clear(Ekeko_Tiler *t);
Ekeko_Rectangle * ekeko_tiler_rects_get(Ekeko_Tiler *t);
void ekeko_tiler_rects_free(Ekeko_Rectangle *rects);

extern Ekeko_Tiler_Class tiler_rect_splitter;

#endif

