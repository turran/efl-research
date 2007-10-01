#ifndef _COMPONENT_H
#define _COMPONENT_H

/**
 * @file
 * @brief Components
 * @defgroup Component_Internal_Group Component
 * @ingroup Internal_Group
 * @{
 */

/**
 * To be documented
 * FIXME: To be fixed
 */
struct _Enginy_Component
{
	Enginy_Path		*path; 		/**< Main temporal data */
	Enginy_Component_Reader 	*src; 		/**< Source data */
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
 * To be documented
 * FIXME: To be fixed
 */
enum
{
	EQUIS_COMPONENT_O, 	/**< Output Only Component */
	EQUIS_COMPONENT_IO,	/**< Input/Output Component */
	EQUIS_COMPONENT_TYPES
};

Enginy_Component * enginy_component_new(void);
void enginy_component_notify(Enginy_Component *c);
int enginy_component_generate(Enginy_Component *c, int *num);

/** @} */
#endif
