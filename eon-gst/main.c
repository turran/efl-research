#include <Eon.h>
#include <gst/gst.h>
#include <gst/video/video.h>

Eon_Document *doc;
Eon_Canvas *canvas;
Eon_Buffer *b;

typedef Eon_Paint * (*paint_get)(void);

static Eon_Paint * fade_get(paint_get pg)
{
	Eon_Fade *f;

	f = eon_fade_new(doc);
	//ekeko_object_child_append(canvas, f);
	eon_paint_square_x_rel_set(f, 0);
	eon_paint_square_y_rel_set(f, 0);
	eon_paint_square_w_rel_set(f, 100);
	eon_paint_square_h_rel_set(f, 100);
	eon_transition_step_set(f, 0.5);

	ekeko_object_child_append(f, b);
	ekeko_object_child_append(f, pg());

	return (Eon_Paint *)f;
}

static Eon_Paint * stripes_get(void)
{
	Eon_Stripes *s;

	s = eon_stripes_new(doc);
	//ekeko_object_child_append(canvas, s);
	eon_stripes_color1_set(s, 0xaaaa0000);
	eon_stripes_color2_set(s, 0x55005500);
	eon_paint_square_x_rel_set(s, 0);
	eon_paint_square_y_rel_set(s, 0);
	eon_paint_square_w_rel_set(s, 100);
	eon_paint_square_h_rel_set(s, 100);

	return (Eon_Paint *)s;
}

static Eon_Paint * checker_get(void)
{
	Eon_Checker *ch;

	ch = eon_checker_new(doc);
	//ekeko_object_child_append(canvas, ch);
	eon_checker_color1_set(ch, 0x55005500);
	eon_checker_color2_set(ch, 0x33003300);
	eon_paint_square_x_rel_set(ch, 0);
	eon_paint_square_y_rel_set(ch, 0);
	eon_paint_square_w_rel_set(ch, 100);
	eon_paint_square_h_rel_set(ch, 100);

	return (Eon_Paint *)ch;
}

static Eon_Paint * compound_get(paint_get pg)
{
	Eon_Compound *c;
	Eon_Compound_Layer *cl;

	c = eon_compound_new(doc);
	//ekeko_object_child_append(canvas, c);
	eon_paint_square_x_rel_set(c, 0);
	eon_paint_square_y_rel_set(c, 0);
	eon_paint_square_w_rel_set(c, 100);
	eon_paint_square_h_rel_set(c, 100);

	cl = eon_compound_layer_new(doc);
	ekeko_object_child_append(c, cl);
	eon_compound_layer_rop_set(cl, ENESIM_FILL);
	eon_compound_layer_paint_set(cl, b);

	cl = eon_compound_layer_new(doc);
	ekeko_object_child_append(c, cl);
	eon_compound_layer_rop_set(cl, ENESIM_BLEND);
	eon_compound_layer_paint_set(cl, pg());

	return (Eon_Paint *)c;
}

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
	//ekeko_object_child_append(canvas, b);
	eon_paint_square_x_rel_set(b, 0);
	eon_paint_square_y_rel_set(b, 0);
	eon_paint_square_w_rel_set(b, 100);
	eon_paint_square_h_rel_set(b, 100);
	//ekeko_renderable_show((Ekeko_Renderable *)b);

	r = eon_rect_new(doc);
	ekeko_object_child_append(canvas, r);
	eon_rect_x_rel_set(r, 25);
	eon_rect_y_rel_set(r, 25);
	eon_rect_w_rel_set(r, 50);
	eon_rect_h_rel_set(r, 50);
	eon_rect_corner_radius_set(r, 30);
	eon_shape_stroke_width_set(r, 2);
	eon_shape_stroke_color_set(r, 0xff000000);
	eon_shape_fill_paint_set(r, compound_get(checker_get));
	//eon_shape_fill_paint_set(r, b);
	//eon_shape_fill_paint_set(r, fade_get(checker_get));
	eon_shape_show(r);
#if 0
	{
		Eon_Circle *c;

		c = eon_circle_new(doc);
		ekeko_object_child_append(canvas, c);
		eon_circle_radius_set(c, 40);
		eon_circle_x_rel_set(c, 10);
		eon_circle_y_rel_set(c, 10);
		eon_shape_stroke_width_set(c, 2);
		eon_shape_stroke_color_set(c, 0xff000000);
		eon_shape_fill_paint_set(c, stripes_get());
		eon_shape_show(c);
	}
#endif
}

static void _read(void *data, void *buf, unsigned int nbyte)
{
	GstBuffer *buffer = *((GstBuffer **)buf);
	Enesim_Converter_Data cdata;
	GstCaps *caps;
	GstStructure *structure;
	gint width, height;


	caps = gst_buffer_get_caps(buffer);
	structure = gst_caps_get_structure (caps, 0);
	gst_structure_get_int(structure, "width", &width);
	gst_structure_get_int(structure, "height", &height);

	cdata.argb8888.plane0 = GST_BUFFER_DATA(buffer);
	eon_buffer_data_width_set(b, width);
	eon_buffer_data_height_set(b, height);
	cdata.argb8888.plane0_stride = width;

	eon_buffer_data_set(b, &cdata);
	eon_buffer_format_set(b, ENESIM_CONVERTER_ARGB8888);
	eon_buffer_data_update(b);

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
	GstElement *pipeline, *codec;
	GstBin *bin;

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

	bin = gst_bin_new(NULL);
	{
		GstElement *capsfilter, *sink, *csc;
		GstCaps *caps = NULL;
		GstGhostPad *pad, *binpad;

		csc = gst_element_factory_make("ffmpegcolorspace", NULL);
		capsfilter = gst_element_factory_make("capsfilter", NULL);
		sink = gst_element_factory_make("fakesink", NULL);

		caps = gst_caps_from_string("video/x-raw-rgb,bpp=32,alpha_mask=0xff000000,red_mask=0x00ff0000,green_mask=0x0000ff00,blue_mask=0x000000ff");
		caps = gst_caps_from_string("video/x-raw-rgb,bpp=32,alpha_mask=0x000000ff,red_mask=0x0000ff00,green_mask=0x00ff0000,blue_mask=0xff000000");
		g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);

		g_object_set(G_OBJECT(sink), "qos", FALSE, NULL);
		g_object_set(G_OBJECT(sink), "sync", TRUE, NULL);
		g_object_set(G_OBJECT(sink), "signal-handoffs", TRUE, NULL);
		g_signal_connect(G_OBJECT(sink), "handoff", G_CALLBACK(_handoff), p);

		gst_bin_add_many(bin, csc, capsfilter, sink, NULL);
		gst_element_link(csc, capsfilter);
		gst_element_link(capsfilter, sink);

		pad = gst_element_get_static_pad (csc, "sink");
		binpad = gst_ghost_pad_new ("sink", pad);
		gst_object_unref (pad);
		gst_element_add_pad(bin, binpad);

	}
	gst_bin_add_many(GST_BIN(pipeline), codec, NULL);
	g_object_set(G_OBJECT(codec), "video-sink", G_OBJECT(bin), NULL);

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
