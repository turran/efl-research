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
	void *data;
} Ekeko_Object_State;

struct _Ekeko_Object
{
	Eina_Inlist list;
	Ekeko_Object_State curr;
	Ekeko_Object_State prev;
	/* possible callbacks */
	// pre?
	// post?
	// generate: make the object generate itself?
	// make objects have two states? previous and current? and also user
	// provided calls to check if the state is the same
	void *data;
};

#endif
