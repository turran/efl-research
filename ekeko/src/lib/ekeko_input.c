#include "Ekeko.h"
#include "ekeko_private.h"
/**
 * TODO
 * code the events support, to receive events from it
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Input * ekeko_input_new(Ekeko_Canvas *c)
{
	Ekeko_Input *i;
	
	i = calloc(1, sizeof(Ekeko_Input));
	i->canvas = c;
	/* attach the input to the list of canvas' inputs */
	ekeko_canvas_input_add(c, i);
	
	return i;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI ekeko_input_feed_mouse_down(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp)
{
	i->last_timestamp = timestamp;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_up(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp)
{
	i->last_timestamp = timestamp;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_move(Ekeko_Input *i, int x, int y, unsigned int timestamp)
{
	Ekeko_Event ev;
	int px, py;
	
	i->last_timestamp = timestamp;
	if (!i->pointer.inside) return;
	
	px = i->pointer.x;
	py = i->pointer.y;
	
	i->pointer.x = x; 
	i->pointer.y = y;
	/* if the pointer is grabbed, the logic is different */
	if (i->pointer.grabbed)
	{
		/* move */
		/* out */
	}
	else
	{
		Ekeko_Object *inside;
		
		inside = ekeko_canvas_object_get_at_coordinate(i->canvas, x, y);
		/* TODO check the visibility */
		if (inside == i->pointer.obj)
		{
			/* move */
			if (inside)
			{
				ekeko_object_event_callback_call(inside, EKEKO_EVENT_MOUSE_MOVE, &ev);
			}
		}
		else
		{
			printf("%p %p\n", inside, i->pointer.obj);
			/* old out */
			if (i->pointer.obj)
			{
				ekeko_object_event_callback_call(i->pointer.obj, EKEKO_EVENT_MOUSE_OUT, &ev);
				i->pointer.obj = NULL;
			}
			/* new in */
			if (inside)
			{
				ekeko_object_event_callback_call(inside, EKEKO_EVENT_MOUSE_IN, &ev);
				i->pointer.obj = inside;
			}
		}
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_in(Ekeko_Input *i, unsigned int timestamp)
{
	Ekeko_Object *inside;
	Ekeko_Event ev;
	
	if (i->pointer.inside) return;
	
	inside = ekeko_canvas_object_get_at_coordinate(i->canvas, i->pointer.x, i->pointer.y);
	if (!inside) return;
	/* call the _in_ callback */
	i->pointer.obj = inside;
	i->pointer.inside = EINA_TRUE;
	i->last_timestamp = timestamp;
	
	ekeko_object_event_callback_call(inside, EKEKO_EVENT_MOUSE_IN, &ev);
	ekeko_input_feed_mouse_move(i, i->pointer.x, i->pointer.y, timestamp);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_out(Ekeko_Input *i, unsigned int timestamp)
{
	i->last_timestamp = timestamp;
	i->pointer.inside = EINA_FALSE;
	if (!i->pointer.grabbed) return;
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_wheel(Ekeko_Input *i, int direction, int z, unsigned int timestamp)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_key_down(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_key_up(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_hold(Ekeko_Input *i, int hold, unsigned int timestamp)
{
	
}

