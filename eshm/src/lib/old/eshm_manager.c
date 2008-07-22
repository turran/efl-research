#include <Ecore.h>
#include <Ecore_Data.h>
#include <E_DBus.h>
#include "Eshm.h"
#include "eshm_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
int manager_new(void)
{
	eshm = calloc(1, sizeof(Eshm));
	if (!eshm) return -1;
	
	eshm->connection = e_dbus_bus_get(DBUS_BUS_SESSION);
	if (!eshm->connection) goto dbus_error;
	
	/* initialize the list of segments */
	eshm->segments = ecore_list_new();
	return 0;
dbus_error:
	free(eshm);
	eshm = NULL;
	return -1;
}

void manager_delete(void)
{
	/* release each segment?, how to do so, if the loop has finished 
	 * do one more iteration ? */ 
	free(eshm);
	eshm = NULL;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
E_DBus_Connection * eshm_manager_connection_get(void)
{
	return eshm->connection;
}
