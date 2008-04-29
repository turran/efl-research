#ifndef CANVAS_H_
#define CANVAS_H_

struct _ESVG
{
	Ekeko_Canvas *canvas;
	ESVG_Shape *shapes;
	ESVG_Engine *engine;
	void *engine_data;
};

void esvg_canvas_shape_add(ESVG *svg, ESVG_Shape *s, Ekeko_Object_Class *class, void *data);
void esvg_canvas_shape_remove(ESVG_Shape *s);

#endif /*CANVAS_H_*/
