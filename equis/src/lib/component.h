#ifndef _COMPONENT_H
#define _COMPONENT_H

/**
 * The component idea is to define a vector pipeline using small black boxes
 * A source component can be a path
 * A sink component 
 * A pipe component can be the transformation matrix operations
 */

struct _Equis_Component
{
	/* input array of vertices/cmd's */
	void *in;
	/* output array of vertices */
	void *out;
	/* in case the output vertices need to be recalculated */
	int has_changed; 	
	
	Equis_Component *from;
	const char 	*type;
	void 		*data;
};

/* forward notification of a change, so dependant components are also marked
 * as changed
 */
void equis_component_change_set(Equis_Component *g, int changed);
void equis_component_attach(Equis_Component *g, Equis_Component *to);
void equis_component_dettach(Equis_Component *g);

#endif
