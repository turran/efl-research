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
EAPI ekeko_input_feed_mouse_down(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp, const void *data)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_up(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_move(Ekeko_Input *i, int x, int y, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_in(Ekeko_Input *i, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_out(Ekeko_Input *i, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_mouse_wheel(Ekeko_Input *i, int direction, int z, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_key_down(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_key_up(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp, const void *data)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_input_feed_hold(Ekeko_Input *i, int hold, unsigned int timestamp, const void *data)
{
	
}

