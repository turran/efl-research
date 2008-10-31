#ifndef CANVAS_H_
#define CANVAS_H_

struct _Ekeko_Canvas
{
	/* possible callbacks */
	// rectangle_push: in case of double buffer, this will inform what has changed (the last rendered rectangle)
	// what about post processing of the canvas??
	Ekeko_Tiler *tiler;
	int tiler_type; /* FIXME fix this to an enum */
	Ekeko_Renderable *renderables;
	Ekeko_Input *inputs;
	Ekeko_Rectangle *damages;
	// obscures
	//Ekeko_Canvas_Class *cclass;
	void *cdata;
	Eina_Rectangle size;
	Eina_List *valid; /* objects that need to be draw */
	Eina_List *invalid; /* objects that dont need to be draw */
	Eina_Bool changed; /* main flag that checks if the canvas has changed in any way */
	Eina_Bool size_changed; /* when what has changed is the size of the canvas */
};


void ekeko_canvas_change(Ekeko_Canvas *c);
void ekeko_canvas_input_add(Ekeko_Canvas *c, Ekeko_Input *i);
Ekeko_Renderable * ekeko_canvas_object_get_at_coordinate(Ekeko_Canvas *c, unsigned int x, unsigned int y);

#endif
