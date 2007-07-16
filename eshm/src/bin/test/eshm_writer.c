#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include <Ecore.h>
#include <Ecore_Data.h>
#include <E_DBus.h>
	
E_DBus_Connection *conn;

struct Writer
{
	E_DBus_Connection *c;
	int num_readers;
};

struct Reader
{
	char *obj;
	int reading;
};

int frames_lost = 0;
int reading = 0;

Ecore_List *readers = NULL;

/* write timer */
int write_data(void *data)
{
	int i, j;
	E_DBus_Connection *c;
	DBusMessage *s;

	c = data;
	if (reading)
	{
		frames_lost++;
		printf("lost\n");
		/* instead of calling again the timer, wait for 
		 * a signal from a reader to start writing again
		 */
		return 1;
	}
	/* emulate a data write */
	for (i = 0; i < 1024; i ++)
		j = i * 2 + 3;
	/* send a signal that we end writing */
	reading = readers->nodes;
	s = dbus_message_new_signal(
		"/org/e/shm/TestWriter",
		"org.e.ipc.writer","DataWrote");
	e_dbus_message_send(conn, s, NULL, -1, NULL);
	return 1;
}
/* signal callbacks */
static void
cb_data_read(void *data, DBusMessage *m)
{
	struct Reader *r;

	r = data;
	printf("client data read %s\n", r->obj);
	reading--;
}

/* methods callbacks */
DBusMessage *
cb_request_read(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusError e;
	DBusMessage *reply;
	char *object;
	struct Reader *reader;
	
	reader = calloc(1, sizeof(struct Reader));
	if (!reader)
	{
		return NULL;
	}
	memset(&e, 0, sizeof(DBusError));
	dbus_message_get_args(msg, &e, DBUS_TYPE_OBJECT_PATH, 
		&object, DBUS_TYPE_INVALID);
	reply = dbus_message_new_method_return(msg);
	reader->obj = strdup(object);
	ecore_list_append(readers, reader);
	/* set the signal handler for this reader */
	printf("[REQUEST READ %s]\n", reader->obj);
	e_dbus_signal_handler_add(conn, "org.e.ipc2", 
		reader->obj, "org.e.ipc.reader", 
		"DataRead", cb_data_read, reader);
	return reply;
}

DBusMessage *
cb_release_read(E_DBus_Object *obj, DBusMessage *msg)
{

}

DBusMessage *
cb_list_readers(E_DBus_Object *obj, DBusMessage *msg)
{
	printf("[LIST READERS]\n");
	return NULL;
}

void
cb_request_name(void *data, DBusMessage *m, DBusError *err)
{
	printf("request name\n");
	printf("%p\n", data);
}


int setup(E_DBus_Connection *c)
{
	E_DBus_Object *o;
	E_DBus_Interface *i;

	readers = ecore_list_new();
	if (!readers) return -ENOMEM;

	e_dbus_request_name(c, "org.e.ipc", 0, cb_request_name, NULL);
	o = e_dbus_object_add(c, "/org/e/shm/TestWriter", NULL);
	/* the writer interface */
	i = e_dbus_interface_new("org.e.ipc.writer");
	
	e_dbus_interface_method_add(i, "RequestRead", DBUS_TYPE_OBJECT_PATH_AS_STRING, NULL, cb_request_read);
	e_dbus_interface_method_add(i, "ListReaders", NULL, NULL, cb_list_readers);
	e_dbus_object_interface_attach(o, i);
	
	return 0;
}

int main(int argc, char **argv)
{

	ecore_init();
	e_dbus_init();
	e_dbus_signal_init();

	conn = e_dbus_bus_get(DBUS_BUS_SESSION);
	if (!conn) goto error;
	
	if (setup(conn) < 0) goto error;

	ecore_timer_add(0.0333, write_data, conn); /* 1/30 = 0.0333 */
	ecore_main_loop_begin();
	e_dbus_connection_unref(conn);
error:
	e_dbus_signal_shutdown();
	e_dbus_shutdown();
	ecore_shutdown();
	return 0;
}
