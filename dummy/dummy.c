/*
 * dummy.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Dummy_Private *)((Dummy *)(d))->private)

#define DUMMY_TYPE_NAME "Dummy"

struct _Dummy_Private
{
	Dummy_Canvas *dc;
};

static void _ctor(void *instance)
{
	Dummy *dc;
	Dummy_Private *prv;

	dc = (Dummy*) instance;
	dc->private = prv = type_instance_private_get(dummy_type_get(), instance);
	/* create the canvas */
	prv->dc = dummy_canvas_new();
	/* TODO mark the canvas as the top most */
	dummy_canvas_root_set(prv->dc);
}

static void _dtor(void *canvas)
{

}

static Dummy *_dummy = NULL;
static Input *_input = NULL;

static int _idler(void *data)
{
	Dummy_Private *prv;
	if (!_dummy)
		return 1;

	/* process the objects!! :) */
	object_process((Object *)_dummy->private->dc);
	return 1;
}

static int _resize_cb(void *data, int type, void *event)
{
	Ecore_Sdl_Event_Video_Resize *e = event;
	Dummy_Private *prv;

	prv = PRIVATE(_dummy);
	dummy_canvas_resize(prv->dc, e->w, e->h);
	return 1;
}
static int _mouse_down_cb(void *data, int type, void *event)
{
	input_feed_mouse_down(_input);
	return 1;
}

static int _mouse_up_cb(void *data, int type, void *event)
{
	input_feed_mouse_up(_input);
	return 1;
}

static int _mouse_move_cb(void *data, int type, void *event)
{
	Ecore_Sdl_Event_Mouse_Move *e = event;

	input_feed_mouse_move(_input, e->x, e->y);
	return 1;
}

static int _in_cb(void *data, int type, void *event)
{
	printf("mouse in\n");
	return 1;
}
static int _out_cb(void *data, int type, void *event)
{
	printf("mouse out\n");
	return 1;
}
static int
_sdl_event(void *data)
{
	ecore_sdl_feed_events();
	return 1;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type *dummy_type_get(void)
{
	static Type *type = NULL;

	if (!type)
	{
		type = type_new(DUMMY_TYPE_NAME, sizeof(Dummy),
				sizeof(Dummy_Private), object_type_get(), _ctor,
				_dtor, NULL);
	}

	return type;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
void dummy_init(void)
{
	/* initialize etk2 */
	main_init();
	ecore_init();
	ecore_sdl_init(NULL);
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);
	/* add the idler */
	ecore_idle_enterer_add(_idler, NULL);
}

void dummy_shutdown(void)
{
	ecore_main_loop_quit();
	main_shutdown();
	ecore_shutdown();
	ecore_sdl_shutdown();
}

Dummy * dummy_new(void)
{
	/* Singleton for now */
	if (!_dummy)
	{
		Dummy_Private *prv;

		_dummy = type_instance_new(dummy_type_get());
		prv = PRIVATE(_dummy);
		/* add the input */
		_input = canvas_input_new((Canvas *)prv->dc);
		/* add the callbacks */
		ecore_event_handler_add(ECORE_SDL_EVENT_RESIZE, _resize_cb, NULL);
		ecore_event_handler_add(ECORE_SDL_EVENT_GOT_FOCUS, _in_cb, NULL);
		ecore_event_handler_add(ECORE_SDL_EVENT_LOST_FOCUS, _out_cb, NULL);
		ecore_event_handler_add(ECORE_SDL_EVENT_MOUSE_BUTTON_DOWN, _mouse_down_cb, NULL);
		ecore_event_handler_add(ECORE_SDL_EVENT_MOUSE_BUTTON_UP, _mouse_up_cb, NULL);
		ecore_event_handler_add(ECORE_SDL_EVENT_MOUSE_MOVE, _mouse_move_cb, NULL);
		/* FIXME Ecore_SDL doesnt support the in/out events, make it always in */
		input_feed_mouse_in(_input);
		/* the event feeder
		 * FIXME evas/ecore has a very weird way to feed sdl events!
		 */
		ecore_timer_add(0.008, _sdl_event, NULL);
	}
	return _dummy;
}

void dummy_loop(void)
{
	printf("loop\n");
	ecore_main_loop_begin();
	printf("quit\n");
}

Dummy_Canvas * dummy_canvas_get(Dummy *d)
{
	Dummy_Private *prv;

	prv = PRIVATE(d);
	return prv->dc;
}
