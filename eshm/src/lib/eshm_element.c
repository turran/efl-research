#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>

#include <Ecore.h>
#include <Ecore_Data.h>
#include <E_DBus.h>
#include "Eshm.h"
#include "eshm_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eshm_Element * _element_new(Eshm_Segment *s, char *name, int offset, int size)
{
	Eshm_Element *e;

	e = calloc(1, sizeof(Eshm_Element));
	e->s = s;
	e->name = strdup(name);
	e->size = size;
	e->data = (void *)((int)s->data + offset);
	printf("new element at %p (offset = %d)\n", e->data, offset);
	ecore_list_append(s->elements, e);

	return e;
}

static void _element_new_cb(void *data, DBusMessage *m, DBusError *err)
{

}

static void _element_new_from_file_cb(void *data, DBusMessage *m, DBusError *err)
{
	Eshm_Callback_Wrapper *wp;
	Eshm_Segment *s;
	Eshm_Element *e;
	
	int offset;
	int size;
	char *name;

	wp = data;
	s = wp->lib_data;

	ERROR(err)
	dbus_message_get_args(m, err,
		DBUS_TYPE_OBJECT_PATH, &name, 
		DBUS_TYPE_INT32, &offset, 
		DBUS_TYPE_INT32, &size,
		DBUS_TYPE_INVALID);
	e = _element_new(s, name, offset, size);
	if (wp->cb)
		wp->cb(e, wp->user_data);

}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

/**
 *
 *
 */
void eshm_element_new_from_file(Eshm_Segment *s, char *file, Eshm_Callback cb, void *data)
{
	Eshm_Callback_Wrapper *wp;
	DBusMessage *m;
	
	wp = calloc(1, sizeof(Eshm_Callback_Wrapper));
	wp->cb = cb;
	wp->user_data = data;
	wp->lib_data = s;

	m = dbus_message_new_method_call("org.e.shm", s->name,
		"org.e.shm.segment", "NewElementFromFile");
	dbus_message_append_args(m, DBUS_TYPE_STRING, &file,
		DBUS_TYPE_INVALID);
	e_dbus_message_send(eshm->connection, m, _element_new_from_file_cb, 
		-1, wp);
}

/**
 *
 *
 */
void eshm_element_new(Eshm_Segment *s, int size, Eshm_Callback cb, void *data)
{
	Eshm_Callback_Wrapper *wp;
	DBusMessage *m;
	
	wp = calloc(1, sizeof(Eshm_Callback_Wrapper));
	wp->cb = cb;
	wp->user_data = data;
	wp->lib_data = s;

	m = dbus_message_new_method_call("org.e.shm", s->name,
		"org.e.shm.segment", "NewElement");
	dbus_message_append_args(m, DBUS_TYPE_INT32, &size,
		DBUS_TYPE_INVALID);
	e_dbus_message_send(eshm->connection, m, _element_new_cb, 
		-1, wp);
}

/**
 *
 *
 */
void * eshm_element_data_get(Eshm_Element *e)
{
	return e->data;
}

/**
 *
 *
 */
char * eshm_element_name_get(Eshm_Element *e)
{
	return e->name;
}

/**
 *
 *
 */
void * eshm_element_size_get(Eshm_Element *e)
{
	return e->size;
}


