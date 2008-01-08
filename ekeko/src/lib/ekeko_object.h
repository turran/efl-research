#ifndef EKEKO_OBJECT_H_
#define EKEKO_OBJECT_H_

/* every object should receive the area or areas to draw based on their
 * boundings
 */

struct _Ekeko_Object
{
	Eina_Inlist list;
	Eina_Rectangle r;
	/* possible callbacks */
	// pre?
	// post?
	// generate: make the object generate itself?
	// make objects have two states? previous and current? and also user
	// provided calls to check if the state is the same
};

#endif
