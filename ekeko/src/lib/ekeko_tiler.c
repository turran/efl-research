#include "Ekeko.h"
#include "ekeko_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Tiler *ekeko_tiler_new(int type, int w, int h)
{
	Ekeko_Tiler *t;
	Ekeko_Tiler_Class *c;
	
	switch (type)
	{
		case EKEKO_TILER_SPLITTER:
		c = &tiler_rect_splitter;
		break;

		case EKEKO_TILER_TILEBUF:
		c = &tiler_tilebuf;
		break;
		
		default:
		return NULL;
	}
	t = calloc(1, sizeof(Ekeko_Tiler));
	t->canvas.w = w;
	t->canvas.h = h;
	t->tile.w = 8;
	t->tile.h = 8;
	t->class = c;
	if (c->new)
		t->data = c->new(t);
	return t;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_tiler_free(Ekeko_Tiler *t)
{
	if (t->class->free)
		t->class->free(t);
	free(t);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_tiler_tile_size_set(Ekeko_Tiler *t, int w, int h)
{
	if ((w <= 0) || (h <= 0)) return;

	t->tile.w = w;
	t->tile.h = h;
	if (t->class->tile_size_set)
		t->class->tile_size_set(t, w, h);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
int ekeko_tiler_rect_add(Ekeko_Tiler *t, Eina_Rectangle *r)
{
	if (t->class->rect_add)
		return t->class->rect_add(t, r);
	else
		return 0;

}
/**
 * To be documented
 * FIXME: To be fixed
 */
int ekeko_tiler_rect_del(Ekeko_Tiler *t, Eina_Rectangle *r)
{
	if (t->class->rect_del)
		return t->class->rect_del(t, r);
	else
		return 0;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_tiler_clear(Ekeko_Tiler *t)
{
	if (t->class->clear)
		t->class->clear(t);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Rectangle * ekeko_tiler_rects_get(Ekeko_Tiler *t)
{
	if (t->class->rects_get)
		return t->class->rects_get(t);
	else
		return NULL;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_tiler_rects_free(Ekeko_Rectangle *rects)
{
	while (rects)
	{
		Ekeko_Rectangle *r;

		r = rects;
		rects = eina_inlist_remove(EINA_INLIST_GET(rects), EINA_INLIST_GET(r));
		free(r);
	}
}
