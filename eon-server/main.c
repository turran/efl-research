#include <Eon.h>
#include <Ecore.h>
#include <Ecore_Ipc.h>

#include <stdio.h>

typedef struct _Eon_Server
{
	Eon_Document *doc;
	Ecore_Ipc_Server *server;
} Eon_Server;

typedef enum _Eon_Server_Major
{
	OBJECT_ADD,
	OBJECT_DELETE,
	PROPERTY_CHANGE,
} Eon_Server_Major;

int sig_exit_cb(void *data, int ev_type, void *ev)
{
	ecore_main_loop_quit();
	return 1;
}

int handler_client_add(void *data, int ev_type, void *ev)
{
	Ecore_Ipc_Event_Client_Add *e = (Ecore_Ipc_Event_Client_Add *)ev;

	printf("client %p connected to server\n", e->client);

	return 1;
}

int handler_client_del(void *data, int ev_type, void *ev)
{
	Ecore_Ipc_Event_Client_Del *e = (Ecore_Ipc_Event_Client_Del *)ev;

	printf("client %p disconnected from server\n", e->client);

	return 1;
}

int handler_client_data(void *data, int ev_type, void *ev)
{
	Ecore_Ipc_Event_Client_Data *e = (Ecore_Ipc_Event_Client_Data *)ev;

	switch (e->major)
	{
		case OBJECT_ADD:
		printf("Adding object %s\n", e->data);
		break;

		case OBJECT_DELETE:
		printf("Deleting object %s\n", e->data);
		break;

		case PROPERTY_CHANGE:
		printf("Changing property %d on object %d\n", e->minor, e->ref);
		break;

	}
#if 0
	printf("client %p sent [%i] [%i] [%i] (%s)\n", e->client, e->major,
				e->minor, e->size, (char *)e->data);

	ecore_ipc_client_send(e->client, 3, 4, 0, 0, 0, "Pants On!", 9);
#endif
	return 1;
}


int main(void)
{
	Ecore_Ipc_Server *server;
	Eon_Document *doc;
	Eon_Canvas *canvas;
	Eon_Rect *r;

	ecore_init();
	ecore_ipc_init();
	eon_init();

	/* app setup */    
	ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, sig_exit_cb, NULL);
	/* server setup */
	server = ecore_ipc_server_add(ECORE_IPC_LOCAL_SYSTEM, "eon-remote", 0, NULL);
	ecore_event_handler_add(ECORE_IPC_EVENT_CLIENT_ADD, handler_client_add, NULL);
	ecore_event_handler_add(ECORE_IPC_EVENT_CLIENT_DEL, handler_client_del, NULL);
	ecore_event_handler_add(ECORE_IPC_EVENT_CLIENT_DATA, handler_client_data, NULL);
	/* gfx setup */
	doc = eon_document_new("sdl", 640, 480, "resizable");
	canvas = eon_document_canvas_get(doc);
	eon_canvas_x_rel_set(canvas, 0);
	eon_canvas_y_rel_set(canvas, 0);
	eon_canvas_w_rel_set(canvas, 100);
	eon_canvas_h_rel_set(canvas, 100);

	r = eon_rect_new(canvas);
	eon_rect_fill_color_set(r, 0xffffffff);
	eon_shape_draw_mode_set((Eon_Shape *)r, ENESIM_SHAPE_DRAW_MODE_FILL);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	eon_rect_show(r);

	ecore_main_loop_begin();

	ecore_ipc_server_del(server);
	ecore_ipc_shutdown();
	ecore_shutdown();

	return 0;
}
