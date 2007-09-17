#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Equis_Component * equis_component_new(void)
{
	Equis_Component *g;

	g = calloc(1, sizeof(Equis_Component));
	return g;
}

void equis_component_reader_new(Equis_Component *c, Equis_Component_Reader *r)
{
	/* append the reader to the list of readers */
}

void equis_component_reader_notify(Equis_Component *c)
{
	/* for each reader call the notify function on the readers 
	 * equis_reader_notify(r) 
	 */
}

/* should create maximum num vertices and store it */
void equis_component_generate(Equis_Component *c, int *num)
{

}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void equis_component_delete(Equis_Component *c)
{

}
