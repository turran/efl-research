#ifndef EKEKO_OBJECT_H_
#define EKEKO_OBJECT_H_

/* every object should receive the area or areas to draw based on their
 * boundings
 */

/* do we need this state fixed with priv data on each or better make
 * callbacks like _state_has_changed and store both states as void *
 */
typedef struct _Ekeko_Object_State
{
	Eina_Rectangle geometry;
	Eina_Bool visible;
	void *data;
} Ekeko_Object_State;

struct _Ekeko_Object
{
	Eina_Inlist list;
	Ekeko_Object_Class *oclass;
	Ekeko_Canvas *canvas;

	Ekeko_Object_State curr;
	Ekeko_Object_State prev;
	Eina_Bool delete_me;
	void *data;
};

#endif
