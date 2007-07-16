#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>

#include <Ecore.h>
#include <Ecore_Data.h>
#include <E_DBus.h>
#include "Eshm.h"
#include "eshm_private.h"

#if 0
/*============================================================================*
 *                              Deprecated                                    * 
 *============================================================================*/
/* things usable but doesnt make much sense here */
static void _segment_get_all_cb(void *data, DBusMessage *m, DBusError *e)
{
	Eshm_Callback_Wrapper *wp;
	Ecore_List *l = NULL;
	
	DBusMessageIter it, it_array;
	int type;

	wp = data;
	
	/* create a list with the elements received */
	/* iterate through the array of segments and store them */
	dbus_message_iter_init(m, &it);
	type = dbus_message_iter_get_arg_type(&it);
	if (type != DBUS_TYPE_ARRAY)
		goto empty;
	dbus_message_iter_recurse(&it, &it_array);
	type = dbus_message_iter_get_arg_type(&it_array);
	if (type != DBUS_TYPE_STRUCT)
		goto empty;
	l = ecore_list_new();
	while ((type = dbus_message_iter_get_arg_type(&it_array)) != DBUS_TYPE_INVALID)
	{
		Eshm_Segment *s;
		DBusMessageIter it_struct;

		int st_type;
		char *name;
		int  size;
		int shmid;
		
		/* iterate through the struct */
		dbus_message_iter_recurse(&it_array, &it_struct);
		dbus_message_iter_get_basic(&it_struct, &name);
		dbus_message_iter_next(&it_struct);
		dbus_message_iter_get_basic(&it_struct, &size);
		dbus_message_iter_next(&it_struct);
		dbus_message_iter_get_basic(&it_struct, &shmid);

		/* iterate through the list of already known segments */
		printf("name %s size %d shmid %d\n", name, size, shmid); 
		//s = segment_new(name, size, shmid);
		// ecore_list_append(l, s);
		dbus_message_iter_next(&it_array);
	}
empty:
	if (wp->cb)
		wp->cb(l, wp->user_data);
	free(wp);
}

void eshm_segment_get_all(Eshm_Callback cb, void *data)
{
	Eshm_Callback_Wrapper *wp;
	DBusMessage *m;
	
	wp = calloc(1, sizeof(Eshm_Callback_Wrapper));
	wp->cb = cb;
	wp->user_data = data;
	m = dbus_message_new_method_call("org.e.shm","/org/e/shm/Manager",
		"org.e.shm.manager", "ListSegments");
	e_dbus_message_send(eshm->connection, m, _segment_get_all_cb, -1, wp);
}


#endif

/*============================================================================*
 *                               Protoypes                                    * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Eshm_Segment * segment_new(char *name, int size, int shmid)
{
	Eshm_Segment *s;
	
	s = calloc(1, sizeof(Eshm_Segment));
	s->name = strdup(name);
	s->size = size;
	s->shmid = shmid;

	s->elements = ecore_list_new();
	/* attach the segment to this process */
	s->data = shmat(s->shmid, NULL, 0);
	if (s->data == (char *)-1)
	{
		/* error getting the segment ? */
		printf("error getting the segment for id %d [%s]\n", s->shmid, strerror(errno));
	}
	return s;
}
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _segment_new_cb(void *data, DBusMessage *m, DBusError *e)
{
	Eshm_Callback_Wrapper *wp;
	Eshm_Segment *s;
	char *name;
	int size;
	int shmid;

	wp = data;
	name = wp->lib_data;

	ERROR(e)
	dbus_message_get_args(m, e, 
		DBUS_TYPE_INT32, &size, 
		DBUS_TYPE_INT32, &shmid,
		DBUS_TYPE_INVALID);
	s = segment_new(name, size, shmid);
	if (!s)
	{
		/* FIXME error allocating the segment */
	}
	else
	{
		ecore_list_append(eshm->segments, s);
	}
	if (wp->cb)
		wp->cb(s, wp->user_data);
	/* store it on the list of segments in case it doesnt exist yet */ 
	free(wp->lib_data);
	free(wp);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

/**
 *
 */
void eshm_segment_new(char *path, int size, Eshm_Callback cb, void *data)
{
	Eshm_Callback_Wrapper *wp;
	DBusMessage *m;
	char *tmp = path;
	
	wp = calloc(1, sizeof(Eshm_Callback_Wrapper));
	wp->cb = cb;
	wp->user_data = data;
	wp->lib_data = strdup(path);

	m = dbus_message_new_method_call("org.e.shm","/org/e/shm/Manager",
		"org.e.shm.manager", "NewSegment");
	dbus_message_append_args(m, DBUS_TYPE_OBJECT_PATH, &path,
		DBUS_TYPE_INT32, &size, 
		DBUS_TYPE_INVALID);
	e_dbus_message_send(eshm->connection, m, _segment_new_cb, -1, wp);
}

/**
 *
 *
 */
char * eshm_segment_name_get(Eshm_Segment *s)
{
	return s->name;
}

/**
 *
 *
 */
int eshm_segment_size_get(Eshm_Segment *s)
{
	return s->size;
}

