#ifndef _COMPONENT_H
#define _COMPONENT_H

struct _Equis_Component
{
	Equis_Point 	*points;
	char 		*cmds;
	Edata_List	*readers;
	/* in case the output vertices need to be recalculated */
	int has_changed;
	/* number of vertices calculated */
	int num_calculated;
	int num_points;
	/* component specific data */
	const char 	*type; /**< component's type */
	void 		*data; /**< specific component data */
};

Equis_Component * equis_component_new(void);
void equis_component_reader_new(Equis_Component *c, Equis_Component_Reader *r);
void equis_component_reader_notify(Equis_Component *c);

#endif
