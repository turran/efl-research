#include <stdio.h>
#include <stdlib.h>

#include <Ecore.h>
#include <E_DBus.h>

E_DBus_Connection *conn;

void
cb_signal_reply(void *data, DBusMessage *reply, DBusError *e)
{
	printf("cb signal reply\n");
}

/* signal callback */
static void
cb_data_wrote(void *data, DBusMessage *m)
{
	DBusMessage *s;
	int i, j;

	printf("ready to read data\n");
	for (i = 0; i < 1024; i++)
	{
		j = i * 3 + 4;
	}
	s = dbus_message_new_signal("/org/e/shm/TestReader","org.e.ipc.reader","DataRead");
	printf("%p\n", s);
	e_dbus_message_send(conn, s, cb_signal_reply, -1, NULL);
	printf("data read\n");
}

void
cb_attach_reply(void *data, DBusMessage *reply, DBusError *e)
{
	printf("[CLIENT ATTACHED]\n");
	if (dbus_error_is_set(e))
	{
		printf("error %s %s\n", e->name, e->message);
		return;
	}
	/* we are attached to the writer, lets wait for the write
	 * signal to start reading
	 */
	e_dbus_signal_handler_add(conn, "org.e.ipc", 
		"/org/e/shm/TestWriter",
		"org.e.ipc.writer", 
		"DataWrote", 
		cb_data_wrote, NULL);
}

void
cb_request_read_reply(void *data, DBusMessage *reply, DBusError *e)
{
	printf("[CLIENT REQUESTED READ]\n");
	if (dbus_error_is_set(e))
	{
		printf("error %s %s\n", e->name, e->message);
		return;
	}
}

void
cb_request_name(void *data, DBusMessage *m, DBusError *e)
{
	if (dbus_error_is_set(e))
	{
		printf("error %s %s\n", e->name, e->message);
		return;
	}
	printf("request name\n");
	printf("%p\n", data);
}

int setup(E_DBus_Connection *c)
{
	E_DBus_Object *o;
	E_DBus_Interface *i;
	DBusMessage *m;
	char *object = "/org/e/shm/TestReader";

	e_dbus_request_name(c, "org.e.ipc2", 0, cb_request_name, NULL);
	o = e_dbus_object_add(c, object, NULL);
	/* the reader interface */
	i = e_dbus_interface_new("org.e.ipc.reader");
	/* write something to the writer */	
	e_dbus_object_interface_attach(o, i);
	m = dbus_message_new_method_call("org.e.ipc","/org/e/shm/TestWriter",
		"org.e.ipc.writer", "RequestRead");
	dbus_message_append_args(m, DBUS_TYPE_OBJECT_PATH, &object, 
		DBUS_TYPE_INVALID);
	e_dbus_message_send(c, m, cb_attach_reply, -1, NULL);
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

	ecore_main_loop_begin();
	e_dbus_connection_unref(conn);
error:
	e_dbus_signal_shutdown();
	e_dbus_shutdown();
	ecore_shutdown();
	return 0;
}
