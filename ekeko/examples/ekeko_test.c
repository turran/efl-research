#include "Ekeko.h"
#include "ekeko_test.h"

Ekeko_Document *d;
Ekeko_Input *input;

#if 0
Canvas *c;
Subcanvas *subcanvas;
Object *background1;
Object *background2;
Object *rectangle1;
Object *rectangle2;
Object *filter1;
/**
 * TODO
 * clean up the sdl_rectangle[class] code, abstract correctly the ekeko_object (WIP)
 * make this app test different things evas can't handle, like new object types (filters, subcanvas, mirror objects, etc)
 * add support for benchmarking different tilers (rect_splitter, tilebuf, etc)
 * 
 */

#define CANVAS_W 640
#define CANVAS_H 480

void rectangle_mouse_in_cb(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	printf("called mouse in on moving rectangle\n");
	ekeko_object_hide(o);
}

void filter_mouse_in_cb(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	ekeko_object_show(rectangle1->object);
}

void rectangle_mouse_out_cb(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	printf("called mouse out on moving rectangle\n");
}

void rectangle_move_cb(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	Eina_Rectangle r1;
	Eina_Rectangle r2;
	Object *obj;
	
	obj = subcanvas_object_get(subcanvas);
	ekeko_object_geometry_get(o, &r1);
	ekeko_object_geometry_get(obj->object, &r2);
	ekeko_object_move(obj->object, r1.y + 10, r1.x); 
}

void init_scene(void)
{
#if 0
	c = canvas_new(CANVAS_W, CANVAS_H);
	/* background */
	background1 = rectangle_new(c);
	object_move(background1, 0, 0);
	object_resize(background1, CANVAS_W, CANVAS_H);
	object_color_set(background1, RGBA(255, 255, 255, 255));
	ekeko_object_show(background1->object);
	/* object moving */
	rectangle1 = rectangle_new(c);
	object_move(rectangle1, 0, 0);
	object_resize(rectangle1, 50, 50);
	object_color_set(rectangle1, RGBA(255, 0, 0, 255));
	ekeko_object_show(rectangle1->object);
	/* subcanvas */
	subcanvas = subcanvas_new(c, 120, 10, 320, 240);
	background2 = rectangle_new(subcanvas_canvas_get(subcanvas));
	object_move(background2, 0, 0);
	object_resize(background2, 320, 240);
	object_color_set(background2, RGBA(0, 255, 255, 255));
	ekeko_object_show(background2->object);
	ekeko_object_show((subcanvas_object_get(subcanvas))->object);
	/* filter moving */
	filter1 = filter_new(c);
	//filter1 = rectangle_new(c);
	object_move(filter1, 10, 10);
	object_resize(filter1, 50, 50);
	object_color_set(filter1, RGBA(16, 123, 255, 255));
	ekeko_object_show(filter1->object);
	/* input */
	input = ekeko_input_new(c->canvas);
	ekeko_object_event_callback_add(filter1->object, EKEKO_EVENT_MOUSE_IN, filter_mouse_in_cb, NULL);
	ekeko_object_event_callback_add(rectangle1->object, EKEKO_EVENT_MOUSE_IN, rectangle_mouse_in_cb, NULL);
	ekeko_object_event_callback_add(rectangle1->object, EKEKO_EVENT_MOUSE_OUT, rectangle_mouse_out_cb, NULL);
	ekeko_object_event_callback_add(filter1->object, EKEKO_EVENT_MOVE, rectangle_move_cb, NULL);
#endif
}


int end = 0;

void sdl_loop(void)
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_ACTIVEEVENT:
			if (event.active.gain)
				ekeko_input_feed_mouse_in(input, 0);
			else
				ekeko_input_feed_mouse_out(input, 0);
			break;
		case SDL_MOUSEMOTION:
			ekeko_input_feed_mouse_move(input, event.motion.x, event.motion.y, 0);        
			break;
		case SDL_MOUSEBUTTONDOWN:
			ekeko_input_feed_mouse_move(input, event.button.x, event.button.y, 0);
			ekeko_input_feed_mouse_down(input, event.button.state, EKEKO_BUTTON_NONE, 0);
			break;
		case SDL_MOUSEBUTTONUP:
			ekeko_input_feed_mouse_move(input, event.button.x, event.button.y, 0);
			ekeko_input_feed_mouse_up(input, event.button.state, EKEKO_BUTTON_NONE, 0);
			break;
		case SDL_VIDEORESIZE:
			ekeko_canvas_geometry_set(c->canvas, event.resize.w, event.resize.h);
			object_resize(background1, event.resize.w, event.resize.h);
			break;
		case SDL_QUIT:
			end = 1;
			break;
		default:
			break;
		}
	}	
}

void loop(void)
{
	Eina_Rectangle r;
	int i = 0;
	int j = 0;
	int k = 0;
	
	while (!end)
	{
		sdl_loop();
		/* TODO this should be inside canvas process, do we actually need to call it? */
		canvas_process(subcanvas_canvas_get(subcanvas));
		canvas_process(c);
		i++;
		j++;
		k++;
		/* move rectangle1 */
		if (i > 10000)
		{
			/* FIXME abstract this */
			ekeko_object_geometry_get(rectangle1->object, &r);
			r.x = (r.x + 1) % CANVAS_W;
			if (r.x == 0)
			{
				r.y = (r.y + 1) % CANVAS_H;
			}
			object_move(rectangle1, r.x, r.y);
			i = 0;
		}
#if 1
		/* move filter1 */
		if (k > 4000)
		{
			/* FIXME abstract this */
			ekeko_object_geometry_get(filter1->object, &r);
			r.x = (r.x + 1) % CANVAS_W;
			if (r.x == 0) {
				r.y = (r.y + 1) % CANVAS_H;
			}
			object_move(filter1, r.x, r.y);
			k = 0;
		}
#endif
		/* move subcanvas (we need to implement the damages first) */
#if 0
		if (j > 20000)
		{
			/* FIXME abstract this */
			ekeko_object_geometry_get(rectangle1->object, &r);
			r.y = (r.y + 1) % CANVAS_H;
			if (r.x == 0) {
				r.x = (r.x + 1) % CANVAS_W;
			}
			object_move(subcanvas_object_get(subcanvas), r.x, r.y);
			j = 0;
		}
#endif
	}
	SDL_Quit();
}
#endif
static void _mutation_cb(Ekeko_Event *e)
{
	printf("Mutation event!!!\n");
}

void main_loop(void)
{
	for(;;)
	{
		ekeko_node_process((Ekeko_Node *)d);
	}
}
/*
 * Scene
 */
static void scene_init(void)
{
	Ekeko_Element *canvas, *obj;
	Ekeko_Value val;

	d = ekeko_document_new("test", "1");
	canvas = ekeko_document_element_new(d, "test", "canvas");
	ekeko_canvas_size_set(canvas, 500, 500);
	obj = ekeko_document_element_new(d, "test", "rect");
	ekeko_node_child_append(d, canvas);
	ekeko_node_child_append(canvas, obj);
	ekeko_value_int_from(&val, 150);
	ekeko_node_event_listener_add(canvas, "DOMAttrModified",  _mutation_cb,
		EINA_FALSE);
	ekeko_value_rectangle_coords_from(&val, 10, 10, 150, 150);
	ekeko_element_attribute_set(obj, "geom", &val);
}

static void scene_shutdown(void)
{
	//subcanvas_delete(s);
	//object_delete(background);
	//object_delete(rectangle);
}
/*
 * Document Type
 */
static void register_init(void)
{
	Ekeko_Document_Type *dt;

	dt = ekeko_document_type_register("test");
	test_canvas_register(dt);
	test_rect_register(dt);
}
static void register_shutdown(void)
{
	
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int main(int argc, char **argv)
{
	ekeko_init();
	SDL_Init(SDL_INIT_VIDEO);
	/* register types */
	register_init();
	/* scene */
	scene_init();
	main_loop();
	scene_shutdown();
	register_shutdown();
	ekeko_shutdown();
	return 0;
}


