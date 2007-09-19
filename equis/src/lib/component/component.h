#ifndef _COMPONENT_H
#define _COMPONENT_H

/**
 *
 *
 */
struct _Equis_Component
{
	Equis_Path		*path; 		/**< Main temporal data */
	Equis_Component_Reader 	*src; 		/**< Source data */
	Edata_List		*readers;
	int 			has_changed; 	/**< Forward notification */
	int 			type;
	/* component specific data */
	const char 		*name; 		/**< Component's name */
	void 			*data; 		/**< Specific component data */
	void 			(*generate)(void *data, int *num);
	void 			(*free)(void *data);
};

/**
 *
 */
enum
{
	EQUIS_COMPONENT_O,
	EQUIS_COMPONENT_IO,
	EQUIS_COMPONENT_TYPES
};

Equis_Component * equis_component_new(void);
void equis_component_notify(Equis_Component *c);
int equis_component_generate(Equis_Component *c, int *num);

#endif
