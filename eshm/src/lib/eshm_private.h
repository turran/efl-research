#ifndef _ESHM_PRIVATE_H
#define _ESHM_PRIVATE_H

#include <Ecore_Data.h>


#define ERROR(e) \
if (dbus_error_is_set(e)) \
{ \
	printf("error %s %s\n", e->name, e->message); \
	return; \
}
 

/* DBUS SPEC */
#define BUS 	"org.e.shm"
#define IFACE 	"org.e.shm.manager"
#define MGR 	"org/e/shm/Manager"
#define M_SEGMENTLIST 	"ListSegment"

struct _Eshm
{
	E_DBus_Connection *connection;
	Ecore_List *segments;
};

struct _Eshm_Segment
{
	Ecore_List *elements;
	
	char 	*name;
	int 	size;
	int 	shmid;
	void 	*data;
};

struct _Eshm_Element
{
	Eshm_Segment 	*s;
	
	char  		*name;
	int 		size;
	void 		*data;
};

typedef struct _Eshm_Callback_Wrapper Eshm_Callback_Wrapper;
struct _Eshm_Callback_Wrapper
{
	Eshm_Callback cb;
	void *lib_data;
	void *user_data;
};

extern Eshm *eshm;

#endif
