#ifndef EKEKO_INPUT_H_
#define EKEKO_INPUT_H_

struct _Ekeko_Input
{
	Eina_Inlist list;
	Ekeko_Canvas *canvas; /* canvas associated with this input */
	struct {
		unsigned char inside : 1;
		int mouse_grabbed;
		unsigned int button;
		unsigned int x;
		unsigned int y;
#if 0
		struct {
			Evas_List *in;
		} object;
#endif
	} pointer;
	struct {
		
	} keyboard;
#if 0
	Ekeko_Modifier  modifiers;
	Ekeko_Lock      locks;
#endif
	unsigned int   last_timestamp;
	int            last_mouse_down_counter;
	int            last_mouse_up_counter;
};


#endif /*EKEKO_INPUT_H_*/
