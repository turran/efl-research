#include <Eon.h>
#include <gst/gst.h>
#include <gst/video/video.h>

Eon_Document *doc;
Eon_Canvas *canvas;
Eon_Buffer *b;

/* create the eon scene */
static void scene_create(void)
{
	Eon_Rect *r;

	doc = eon_document_new("sdl", 640, 480, "resizable");
        canvas = eon_document_canvas_get(doc);

	eon_canvas_x_rel_set(canvas, 0);
	eon_canvas_y_rel_set(canvas, 0);
	eon_canvas_w_rel_set(canvas, 100);
	eon_canvas_h_rel_set(canvas, 100);

	r = eon_rect_new(doc);
	ekeko_object_child_append(canvas, r);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	eon_shape_fill_color_set(r, 0xffffffff);
	eon_shape_show(r);
	
	b = eon_buffer_new(doc);
}

static void _read(void *data, void *buf, unsigned int nbyte)
{
	GstBuffer  *buffer = *((GstBuffer **)buf);
	Enesim_Converter_Data cdata;

	//eon_buffer_data_set(b, &cdata);
	gst_buffer_unref(buffer);
}

/* here we should write data to the pipe */
void
_handoff(GstElement *sink, GstBuffer *buffer, GstPad *pad, gpointer user_data)
{
	Ecore_Pipe *pipe = user_data;

	gst_buffer_ref(buffer);
	ecore_pipe_write(pipe, &buffer, sizeof(buffer));
}

/* create the gst pipe */
static GstElement * pipe_create(char *uri)
{
	Ecore_Pipe *p;
	GstElement *pipeline, *codec, *fsrc, *sink;

	p = ecore_pipe_add (_read, b);

	pipeline = gst_pipeline_new("pipeline");
	if (!pipeline)
		return NULL;

	codec = gst_element_factory_make("playbin", NULL);
	if (!codec)
	{
		gst_object_unref(GST_OBJECT(pipeline));
		return NULL;
	}
	g_object_set(G_OBJECT(codec), "uri", uri, NULL);

	sink = gst_element_factory_make("fakesink", NULL);
	if (!sink)
	{
		gst_object_unref(GST_OBJECT(pipeline));
		return NULL;
	}
	g_object_set(G_OBJECT(sink), "sync", TRUE, NULL);
	g_object_set(G_OBJECT(sink), "signal-handoffs", TRUE, NULL);
	g_signal_connect(G_OBJECT(sink), "handoff", G_CALLBACK(_handoff), p);
	gst_bin_add_many(GST_BIN(pipeline), codec, NULL);
	
	g_object_set(G_OBJECT(codec), "video-sink", sink, NULL);

	return pipeline;
}

int main(int argc, char **argv)
{
	char *uri;
	GstElement *pipeline;

	if (argc != 2)
	{
		printf("eon-gst URI\n");
		return 1;
	}
	uri = argv[1];
	printf("running with uri %s\n", uri);

	eon_init();
	scene_create();
	gst_init(&argc, &argv);
	pipeline = pipe_create(uri);
	if (!pipeline)
	{
		printf("error creating the pipe!!\n");
		return 1;
	}
	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	eon_loop();
	gst_deinit();
	eon_shutdown();
	return 0;
}
