#include <string.h>

#include "Ekeko.h"
#include "ekeko_private.h"

typedef struct _Tilebuf_Tile Tilebuf_Tile;
struct _Tilebuf_Tile {
	unsigned char redraw : 1;
};

typedef struct _tiles {
	int w, h;
	Tilebuf_Tile *tiles;
} tiles_t;

static inline Tilebuf_Tile * TILE(Ekeko_Tiler *t, int x, int y) {
	tiles_t *ts = t->data;

	return &ts->tiles[y * t->tile.w + x];
}

//#define TILE(tb, x, y) ((tb)->tiles.tiles[((y) * (tb)->tiles.w) + (x)])

static int tilebuf_intersect(int tsize, int tlen, int tnum, int x, int w,
		int *x1, int *x2, int *x1_fill, int *x2_fill) {
	int p1, p2;

	/* initial clip out of region */
	if ((x + w) <= 0)
		return 0;
	if (x >= tlen)
		return 0;

	/* adjust x & w so it all fits in region */
	if (x < 0) {
		w += x;
		x = 0;
	}
	if (w < 0)
		return 0;
	if ((x + w) > tlen)
		w = tlen - x;

	/* now figure if the first edge is fully filling its tile */
	p1 = (x) / tsize;
	if ((p1 * tsize) == (x))
		*x1_fill = 1;
	else
		*x1_fill = 0;
	*x1 = p1;

	/* now figure if the last edge is fully filling its tile */
	p2 = (x + w - 1) / tsize;
	if (((p2 + 1) * tsize) == (x + w))
		*x2_fill = 1;
	else
		*x2_fill = 0;
	*x2 = p2;

	return 1;
	tnum = 0;
}

static int tilebuf_x_intersect(Ekeko_Tiler *t, int x, int w, int *x1, int *x2,
		int *x1_fill, int *x2_fill) {
	tiles_t *ts = t->data;
	return tilebuf_intersect(t->tile.w, t->canvas.w, ts->w, x, w, x1, x2,
			x1_fill, x2_fill);
}

static int tilebuf_y_intersect(Ekeko_Tiler *t, int y, int h, int *y1, int *y2,
		int *y1_fill, int *y2_fill) {
	tiles_t *ts = t->data;
	return tilebuf_intersect(t->tile.h, t->canvas.h, ts->h, y, h, y1, y2,
			y1_fill, y2_fill);
}

/* Tiler Class API */
static void * _new(Ekeko_Tiler *t) {
	tiles_t *ts;

	ts = malloc(sizeof(tiles_t));
	ts->tiles = NULL;
	ts->w = (t->canvas.w + (t->tile.w - 1)) / t->tile.w;
	ts->h = (t->canvas.h + (t->tile.h - 1)) / t->tile.h;

	ts->tiles = malloc(ts->w * ts->h * sizeof(Tilebuf_Tile));
#if 0
	if (!tb->tiles.tiles)
	{
		tb->tiles.w = 0;
		tb->tiles.h = 0;
		return;
	}
#endif
	memset(ts->tiles, 0, ts->w * ts->h * sizeof(Tilebuf_Tile));
	return ts;
}

static void _free(Ekeko_Tiler *t) {
	tiles_t *ts = t->data;

	if (ts->tiles)
		free(ts->tiles);
}

static void _tile_size_set(Ekeko_Tiler *t, int w, int h) {

}

static int _rect_add(Ekeko_Tiler *t, Eina_Rectangle *r) {
	Eina_Rectangle r2;
	tiles_t *ts = t->data;
	int tx1, tx2, ty1, ty2, tfx1, tfx2, tfy1, tfy2, xx, yy;
	int num;

	r2.x = r->x;
	r2.y = r->y;
	r2.w = r->w;
	r2.h = r->h;

	if ((r->w <= 0) || (r->h <= 0))
		return 0;
	RECTS_CLIP_TO_RECT(r2.x, r2.y, r2.w, r2.h, 0, 0, t->canvas.w, t->canvas.h);
	if ((r2.w <= 0) || (r2.h <= 0))
		return 0;
	num = 0;
	/* wipes out any motion vectors in tiles it touches into redraws */
	if (tilebuf_x_intersect(t, r2.x, r2.w, &tx1, &tx2, &tfx1, &tfx2)
			&& tilebuf_y_intersect(t, r2.y, r2.h, &ty1, &ty2, &tfy1, &tfy2)) {
		Tilebuf_Tile *tbt;
		int delta_x;
		int delta_y;

		tbt = TILE(t, tx1, ty1);
		delta_x = tx2 - tx1 + 1;
		delta_y = ty2 - ty1 + 1;
		for (yy = delta_y; yy > 0; yy--) {
			Tilebuf_Tile *tbti;

			tbti = tbt;
			for (xx = delta_x; xx > 0; xx--) {
				tbti->redraw = 1;
				tbti++;
			}
			tbt += ts->w;
		}
		num = (tx2 - tx1 + 1) * (ty2 - ty1 + 1);
	}
	return num;
}

static int _rect_del(Ekeko_Tiler *t, Eina_Rectangle *rect) {
	tiles_t *ts = t->data;

	int tx1, tx2, ty1, ty2, tfx1, tfx2, tfy1, tfy2, xx, yy;
	int num;

	num = 0;
	/* wipes out any motion vectors in tiles it touches into redraws */
	if (tilebuf_x_intersect(t, rect->x, rect->w, &tx1, &tx2, &tfx1, &tfx2)
			&& tilebuf_y_intersect(t, rect->y, rect->h, &ty1, &ty2, &tfy1,
					&tfy2)) {
		Tilebuf_Tile *tbt;
		int delta_y;
		int delta_x;

		if (!tfx1)
			tx1++;
		if (!tfx2)
			tx2--;
		if (!tfy1)
			ty1++;
		if (!tfy2)
			ty2--;

		tbt = TILE(t, tx1, ty1);
		delta_x = tx2 - tx1 + 1;
		delta_y = ty2 - ty1 + 1;
		for (yy = delta_y; yy > 0; yy--) {
			Tilebuf_Tile *tbti;

			tbti = tbt;
			for (xx = delta_x; xx > 0; xx--) {
				tbti->redraw = 0;
				tbti++;
			}
			tbt += ts->w;
		}
		num = (tx2 - tx1 + 1) * (ty2 - ty1 + 1);
	}
}

static void _clear(Ekeko_Tiler *t) {
	tiles_t *ts = t->data;
	if (!ts->tiles)
		return;
	memset(ts->tiles, 0, ts->w * ts->h * sizeof(Tilebuf_Tile));

}

static Ekeko_Rectangle * _rects_get(Ekeko_Tiler *t) {
	Ekeko_Rectangle *rects = NULL;
	tiles_t *ts = t->data;
	//Tilebuf_Rect *rects = NULL;
	Tilebuf_Tile *tbt;
	int x, y;

	tbt = TILE(t, 0, 0);
	for (y = 0; y < ts->h; y++) {
		for (x = 0; x < ts->w; x++, tbt++) {
			if (tbt->redraw) {
				Tilebuf_Tile *tbti;
				int can_expand_x = 1, can_expand_y = 1;
				//Tilebuf_Rect *r = NULL;
				Ekeko_Rectangle *r = NULL;
				int xx = 0, yy = 0;
				//r = malloc(sizeof(Tilebuf_Rect));
				r = malloc(sizeof(Ekeko_Rectangle));
				r->list.next = NULL;
				r->list.prev = NULL;
				r->list.last = NULL;

				/* amalgamate tiles */
#if 1
				tbti = tbt;
				while (can_expand_x) {
					tbti++;
					xx++;
					if ((x + xx) >= ts->w)
						can_expand_x = 0;
					else if (!(tbti->redraw))
						can_expand_x = 0;
					if (can_expand_x)
						tbti->redraw = 0;
				}
				tbti = tbt;
				while (can_expand_y) {
					int i;

					tbti += ts->w;
					yy++;
					if ((y + yy) >= ts->h)
						can_expand_y = 0;
					if (can_expand_y) {
						Tilebuf_Tile *tbtj;

						tbtj = tbti;
						for (i = x; i < x + xx; i++, tbtj++) {
							if (!(tbtj->redraw)) {
								can_expand_y = 0;
								break;
							}
						}
					}
					if (can_expand_y) {
						Tilebuf_Tile *tbtj;

						tbtj = tbti;
						for (i = x; i < x + xx; i++, tbtj++)
							tbtj->redraw = 0;
					}
				}
				tbt->redraw = 0;
#else
				xx = 1;
				yy = 1;
#endif

				r->r.x = x * t->tile.w;
				r->r.y = y * t->tile.h;
				r->r.w = (xx) * t->tile.w;
				r->r.h = (yy) * t->tile.h;
				//rects = evas_object_list_append(rects, r);
				rects = eina_inlist_append(rects, r);
				x = x + (xx - 1);
				tbt += xx - 1;
			}
		}
	}
	return rects;
}

Ekeko_Tiler_Class tiler_tilebuf = {
	.new = _new,
	.tile_size_set = _tile_size_set,
	.rect_add = _rect_add,
	.rect_del = _rect_del,
	.clear = _clear,
	.rects_get = _rects_get,
};

