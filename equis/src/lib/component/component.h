#ifndef _COMPONENT_H
#define _COMPONENT_H

/**
 *
 *
 */
struct _Equis_Component
{
	Equis_Path	*path;
#if 0
	Equis_Point 	*points;
	char 		*cmds;
	Equis_Point 	*point_curr;
	char 		*cmd_curr;
	int 		vertex_max; 	/**< number of vertices calculated */
#endif
	
	Edata_List	*readers;
	int 		has_changed; 	/**< usefull for forward notification */
	/* component specific data */
	const char 	*type; 		/**< component's type */
	void 		*data; 		/**< specific component data */
	void 		(*generate)(void *data, int *num);
	void 		(*free)(void *data);
};

Equis_Component * equis_component_new(void);
void equis_component_notify(Equis_Component *c);
int equis_component_generate(Equis_Component *c, int *num);

#endif
