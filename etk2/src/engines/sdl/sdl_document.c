/*
 * eon_document_sdl.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
#include "Ecore_Input.h"
#include "Ecore_Sdl.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _sdl_event(void *data)
{
	ecore_sdl_feed_events();
	return 1;
}

static int _resize_cb(void *data, int type, void *event)
{
	Ecore_Sdl_Event_Video_Resize *e = event;
	Eon_Document *doc = data;

	eon_document_resize(doc, e->w, e->h);

	return 1;
}
static int _mouse_down_cb(void *data, int type, void *event)
{
	Ekeko_Input *input = (Ekeko_Input *)data;
	ekeko_input_feed_mouse_down(input);
	return 1;
}

static int _mouse_up_cb(void *data, int type, void *event)
{
	Ekeko_Input *input = (Ekeko_Input *)data;
	ekeko_input_feed_mouse_up(input);
	return 1;
}

static int _mouse_move_cb(void *data, int type, void *event)
{
	Ekeko_Input *input = (Ekeko_Input *)data;
	Ecore_Event_Mouse_Move *e = event;

	ekeko_input_feed_mouse_move(input, e->x, e->y);
	return 1;
}

static int _in_cb(void *data, int type, void *event)
{
	Ekeko_Input *input = (Ekeko_Input *)data;
	return 1;
}
static int _out_cb(void *data, int type, void *event)
{
	Ekeko_Input *input = (Ekeko_Input *)data;
	return 1;
}

static void * _create(Eon_Document *d)
{
	Ekeko_Input *input;
	Eon_Canvas *canvas;

	printf("[SDL] Initializing SDL\n");
	ecore_sdl_init(NULL);
	SDL_Init(SDL_INIT_VIDEO);

	canvas = eon_document_canvas_get(d);
	/* add the input */
	input = ekeko_canvas_input_new((Ekeko_Canvas *)canvas);
	/* add the callbacks */
	//ecore_event_handler_add(ECORE_SDL_EVENT_RESIZE, _resize_cb, canvas);
	ecore_event_handler_add(ECORE_SDL_EVENT_RESIZE, _resize_cb, d);
	ecore_event_handler_add(ECORE_SDL_EVENT_GOT_FOCUS, _in_cb, input);
	ecore_event_handler_add(ECORE_SDL_EVENT_LOST_FOCUS, _out_cb, input);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_DOWN, _mouse_down_cb, input);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_UP, _mouse_up_cb, input);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_MOVE, _mouse_move_cb, input);
	/* FIXME Ecore_SDL doesnt support the in/out events, make it always in */
	ekeko_input_feed_mouse_in(input);
	/* the event feeder evas/ecore has a very weird way to feed sdl events! */
	ecore_timer_add(0.008, _sdl_event, NULL);
	return NULL;
}

static void _delete(Eon_Document *d, void *data)
{
	ecore_sdl_shutdown();
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Document_Engine eon_document_engine_sdl = {
	.create = _create,
	.delete = _delete,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

