/**
 * parameters:
 * Enesim_Surface *ss
 * Enesim_Rectangle *srect
 * Enesim_Surface *ds
 * Enesim_Rectangle *drect
 * * macros:
 * RENDER_OP_CALL
 */

#ifndef RENDER_OP_CALL
#error "You must define RENDER_OP_CALL before including this header"
#endif

/* TODO use srect and drect */
void 
ARGB8888_NOSCALE_IDENTITY_ITERATOR_PROTO
{
	DATA32 *dp, *sp;
	int h;
	
	dp = ds->data.argb8888.data + (drect->y * ds->w) + drect->x;
	sp = ss->data.argb8888.data + (srect->y * ss->w) + srect->x; 
	h = drect->h;
	while (h--)
	{
		DATA32 *d = dp;
		DATA32 *s = sp;
		DATA32 *e = d + drect->w;

		while (d < e)
		{
			RENDER_OP_CALL
			d++;
			s++;
		}
		dp += ds->w;
		sp += ss->w;
	}
}
