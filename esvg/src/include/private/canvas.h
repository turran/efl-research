#ifndef CANVAS_H_
#define CANVAS_H_

struct _ESVG
{
	Ekeko_Canvas *canvas;
	Eina_Rectangle shape_area;
	ESVG_Rect *background; /* the background rectangle */
	ESVG_Engine *engine;
	void *engine_data;
};

void esvg_canvas_shape_add(ESVG *svg, ESVG_Shape *s, Ekeko_Object_Class *class, void *data);
void esvg_canvas_shape_remove(ESVG_Shape *s);

#endif /*CANVAS_H_*/
