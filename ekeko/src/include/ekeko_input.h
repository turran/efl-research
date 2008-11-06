#ifndef EKEKO_INPUT_H_
#define EKEKO_INPUT_H_

/*
 * @defgroup Ekeko_Input_Group Input
 * @{
 */

typedef enum _Ekeko_Button_Flags
{
	EKEKO_BUTTON_NONE = 0, /**< No extra mouse button data */
	EKEKO_BUTTON_DOUBLE_CLICK = (1 << 0), /**< This mouse button press was the 2nd press of a double click */
	EKEKO_BUTTON_TRIPLE_CLICK = (1 << 1) /**< This mouse button press was the 3rd press of a triple click */
} Ekeko_Button_Flags; /**< Flags for Mouse Button events */


/* TODO the final parameter (data) can be replaced? i think there's no need
 * for it
 */
//EAPI Ekeko_Input * ekeko_input_new(Ekeko_Canvas *c);
EAPI ekeko_input_feed_mouse_down(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_up(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_move(Ekeko_Input *i, int x, int y, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_in(Ekeko_Input *i, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_out(Ekeko_Input *i, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_wheel(Ekeko_Input *i, int direction, int z, unsigned int timestamp);
EAPI void ekeko_input_feed_key_down(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp);
EAPI void ekeko_input_feed_key_up(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp);
EAPI void ekeko_input_feed_hold(Ekeko_Input *i, int hold, unsigned int timestamp); 

/**
 * @}
 */


#endif /*EKEKO_INPUT_H_*/
