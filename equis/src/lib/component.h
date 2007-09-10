#ifndef _GENERATOR_H
#define _GENERATOR_H

struct _Equis_Generator
{
	/* input array of vertices/cmd's */
	void *in;
	/* output array of vertices */
	void *out;
	/* in case the output vertices need to be recalculated */
	int has_changed; 	
	
	Equis_Generator *from;
	const char 	*type;
	void 		*data;
};

/* forward notification of a change, so dependant generators are also marked
 * as changed
 */
void equis_generator_change_set(Equis_Generator *g, int changed);
void equis_generator_attach(Equis_Generator *g, Equis_Generator *to);
void equis_generator_dettach(Equis_Generator *g);

#endif
