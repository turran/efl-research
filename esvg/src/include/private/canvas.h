#ifndef CANVAS_H_
#define CANVAS_H_

struct _ESVG
{
	Ekeko_Canvas *canvas;
	unsigned int width; /* output width */
	unsigned int height; /* output height */
	ESVG_Rect *background; /* the background rectangle */
	Eina_Rectangle shape_area;
	ESVG_Engine *engine;
	ESVG_Engine_Type engine_type;
	void *engine_data;
};

void esvg_canvas_shape_add(ESVG *svg, ESVG_Shape *s, Ekeko_Object_Class *class, void *data);
void esvg_canvas_shape_remove(ESVG_Shape *s);

#endif /*CANVAS_H_*/
