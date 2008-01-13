#include "Ekeko.h"
#include "ekeko_test.h"

/* A subcanvas implements both interfaces: Object and Canvas,
 * so the object_ canvas_ functions are still available
 */
struct _Subcanvas
{
	Object *object;
	Canvas *canvas;
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Subcanvas * subcanvas_new(Canvas *c)
{
	Subcanvas *s;
	
	s = calloc(1, sizeof(Subcanvas));
	//s->canvas = canvas_new(&subcanvas_canvas_class, NULL);
	//s->o = object_new(c, &subcanvas_object_class, NULL);
	return s;
}

Canvas * subcanvas_canvas_get(Subcanvas *s)
{
	return s->canvas;
}

Object * subcanvas_object_get(Subcanvas *s)
{
	return s->object;
}
