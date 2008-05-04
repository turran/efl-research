#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void ekeko_error_set(unsigned int err)
{
	ekeko_err = err;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
unsigned int ekeko_err;
/**
 * 
 */
EAPI char * ekeko_error_to_str(unsigned int err)
{
	switch (err)
	{
	}
	return NULL;
}
