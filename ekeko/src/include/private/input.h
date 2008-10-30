#ifndef INPUT_H_
#define INPUT_H_

struct _Ekeko_Input
{
	Eina_Inlist list;
	Ekeko_Canvas *canvas; /* canvas associated with this input */
	struct {
		Eina_Bool inside;
		int grabbed;
		unsigned int button;
		unsigned int x;
		unsigned int y;
		Ekeko_Object *obj; /* only one object below the cursor */
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
