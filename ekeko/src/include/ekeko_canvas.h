#ifndef EKEKO_CANVAS_H_
#define EKEKO_CANVAS_H_

struct _Ekeko_Canvas
{
	int w;
	int h;
	/* possible callbacks */
	// rectangle_push: in case of double buffer, this will inform what has changed (the last rendered rectangle)
	// what about post processing of the canvas??
	Ekeko_Tiler *tiler;
	Ekeko_Object *objects;
	Ekeko_Input *inputs;
	// damages
	// obscures
	Ekeko_Canvas_Class *cclass;
	void *cdata;
	Eina_Bool changed;
};

void ekeko_canvas_change(Ekeko_Canvas *c);
void ekeko_canvas_input_add(Ekeko_Canvas *c, Ekeko_Input *i);
Ekeko_Object * ekeko_canvas_object_get_at_coordinate(Ekeko_Canvas *c, unsigned int x, unsigned int y);

#endif
