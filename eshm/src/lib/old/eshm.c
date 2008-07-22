#include <Ecore.h>
#include <Ecore_Data.h>
#include <E_DBus.h>
#include "Eshm.h"
#include "eshm_private.h"

static int init = 0;
Eshm *eshm = NULL;

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

/**
 *
 *
 */
int eshm_init(void)
{
	if (++init != 1) return init;
	
	e_dbus_init();
	if (manager_new() < 0) return 0;

	return init;
}

/**
 *
 *
 */
void eshm_shutdown(void)
{
	if (--init) return;
	manager_delete();
	e_dbus_shutdown();
}
