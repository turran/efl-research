#ifndef TILER_H_
#define TILER_H_

typedef struct _Ekeko_Tiler Ekeko_Tiler;
typedef struct _Ekeko_Tiler_Class Ekeko_Tiler_Class;
typedef struct _Ekeko_Rectangle Ekeko_Rectangle;

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
	void * (*new)(Ekeko_Tiler *t);
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
extern Ekeko_Tiler_Class tiler_tilebuf;

/* refactor this */
#ifndef UNLIKELY
#define UNLIKELY(x) (x)
#endif
#define SPANS_COMMON(x1, w1, x2, w2) \
(!((((x2) + (w2)) <= (x1)) || ((x2) >= ((x1) + (w1)))))

#define RECTS_INTERSECT(x, y, w, h, xx, yy, ww, hh) \
((SPANS_COMMON((x), (w), (xx), (ww))) && (SPANS_COMMON((y), (h), (yy), (hh))))

#define RECTS_CLIP_TO_RECT(_x, _y, _w, _h, _cx, _cy, _cw, _ch) \
{ \
   if (RECTS_INTERSECT(_x, _y, _w, _h, _cx, _cy, _cw, _ch)) \
     { \
	if (_x < (_cx)) \
	  { \
	     _w += _x - (_cx); \
	     _x = (_cx); \
	     if (_w < 0) _w = 0; \
	  } \
	if ((_x + _w) > ((_cx) + (_cw))) \
	  _w = (_cx) + (_cw) - _x; \
	if (_y < (_cy)) \
	  { \
	     _h += _y - (_cy); \
	     _y = (_cy); \
	     if (_h < 0) _h = 0; \
	  } \
	if ((_y + _h) > ((_cy) + (_ch))) \
	  _h = (_cy) + (_ch) - _y; \
     } \
   else \
     { \
	_w = 0; _h = 0; \
     } \
}


#endif

