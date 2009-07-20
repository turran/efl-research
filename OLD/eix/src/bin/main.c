#include <string.h>
#include <stdio.h>

#include "Eix.h"
#include "EXML.h"
#include "Ecore_Evas.h"
#include "Etch.h"
#include "Enesim.h"

typedef struct _Eix
{
	EXML *xml;
	Ecore_Evas *ee;
	Evas *e;
	Etch *etch;
	Eina_Inlist *objects;
} Eix;

typedef struct _Eix_Object
{
	EINA_INLIST;
	Evas_Object *o;
	Enesim_Matrix *m;
	Enesim_Surface *src;
	Enesim_Surface *dst;
	Eina_Bool render;
	Enesim_Transformation *tx;
	float angle;
} Eix_Object;

Eix_Object * eix_object_add(Eix *e);
void eix_init(void);
Eix * eix_new(void);
void eix_loop(void);
Eina_Bool eix_file_load(Eix *e, const char *f);


static inline void _transform_apply(Eix_Object *eo)
{
	Eina_Rectangle rect;
	Enesim_Surface_Pixel color;
	Enesim_Surface_Data sdata;
	Enesim_Matrix *matrix;
	int imgw, imgh;
	
	enesim_matrix_rotate(eo->m, eo->angle);
	enesim_surface_data_get(eo->dst, &sdata);
	enesim_surface_size_get(eo->dst, &rect.w, &rect.h);
	enesim_surface_size_get(eo->dst, &imgw, &imgh);
	//memset(sdata.data.argb8888.plane0, 0x0, imgw * imgh * 4);
	eina_rectangle_coords_from(&rect, 0, 0, imgw, imgh);
	//printf("%d %d\n", imgh/2, imgw/2);
	//enesim_transformation_origin_set(o->tx, 0, 0);
	enesim_transformation_origin_set(eo->tx, imgw/2, imgh/2);
#if 1
	matrix = enesim_matrix_new();
	enesim_matrix_translate(matrix, -imgw/2, -imgh/2);
	enesim_matrix_compose(eo->m, matrix, eo->m);
	enesim_matrix_delete(matrix);
#endif
	enesim_transformation_matrix_set(eo->tx, eo->m);

	enesim_surface_pixel_components_from(&color,
		enesim_surface_format_get(eo->dst), 0xaa, 0xff, 0x45, 0x10);
	enesim_transformation_color_set(eo->tx, &color);
//	enesim_transformation_quality_set(eo->tx, ENESIM_FAST);
	enesim_transformation_apply(eo->tx, eo->src, &rect, eo->dst, &rect);
	evas_object_image_pixels_dirty_set(eo->o, 1);

	eo->render = EINA_FALSE;
	enesim_transformation_apply(eo->tx, eo->src, &rect, eo->dst, &rect);
	evas_object_image_pixels_dirty_set(eo->o, 1);
}

void _pre_render(Ecore_Evas *ee)
{
	Eix *eix;
	Eix_Object *eo;

	eix = ecore_evas_data_get(ee, "eix");
	/* iterate over the list of objects */
	EINA_INLIST_FOREACH(eix->objects, eo)
	{
#if 0
		if (eo->render)
			_transform_apply(eo);
#endif
	}
}

static void _document_parse(EXML_Node *n)
{
	
}

void _traverse_script(Eix *e)
{
	
}

void _attributes_shape(Eix *e, const char *tag)
{
	
}

void _childs_shape(Eix *e)
{
	
}

/* Properties callbacks */
void _position_x_uint32(const Etch_Data *curr, const Etch_Data *prev, void *data)
{
	Evas_Object *o = data;
	int x = curr->data.u32;
	int y;

	evas_object_geometry_get(o, NULL, &y, NULL, NULL);
	//printf("moving to %d %d\n", x, y);
	evas_object_move(o, x, y);
}

void _position_y_uint32(const Etch_Data *curr, const Etch_Data *prev, void *data)
{
	Evas_Object *o = data;
	int y = curr->data.u32;
	int x;

	evas_object_geometry_get(o, &x, NULL, NULL, NULL);
	evas_object_move(o, x, y);
}

void _transform_rotate_uint32(const Etch_Data *curr, const Etch_Data *prev, void *data)
{
	Eix_Object *o = data;
	int deg = curr->data.u32;
	
	o->angle = (float)(deg*(3.1416)/180);
	o->render = EINA_TRUE;
	_transform_apply(o);
}

void _transform_scale_uint32(const Etch_Data *curr, const Etch_Data *prev, void *data)
{
	
}

void _time_parse(const char *s, int *sec, int *msec)
{
	//int len = strlen(s);
	char *end;
	int total;
	
	total = strtol(s, &end, 10);
	*sec = total;
	
}

void _traverse_animate_transform(Eix *e, Eix_Object *o)
{
	EXML *xml = e->xml;
	Etch_Animation *ea;
	Etch_Animation_Keyframe *ek;
	char *a;
	int sec, msec;
	int count = 1;
	/* make the evas object use a double buffered object */
	if (!o->src)
	{
		uint32_t *ptr;
	        Enesim_Surface_Data imgdata;
	        int imgw, imgh;
#if 1
		evas_object_image_size_get(o->o, &imgw, &imgh);
		printf("image size %d %d\n", imgw, imgh);
		o->src = enesim_surface_new(ENESIM_SURFACE_ARGB8888, imgw, imgh);
		enesim_surface_data_get(o->src, &imgdata);
		ptr = evas_object_image_data_get(o->o, 0),
		memcpy(imgdata.data.argb8888.plane0, ptr, imgw * imgh * 4);
		o->dst = enesim_surface_new(ENESIM_SURFACE_ARGB8888, imgw, imgh);
		enesim_surface_data_get(o->dst, &imgdata);
		evas_object_image_data_set(o->o, imgdata.data.argb8888.plane0);
		o->tx = enesim_transformation_new();
		o->m = enesim_matrix_new();
		enesim_transformation_rop_set(o->tx, ENESIM_FILL);
#endif
	}
#if 1
	a = exml_attribute_get(xml, "attributeName");
	if (strcmp(a, "transform"))
		return;
	a = exml_attribute_get(xml, "type");
	if (strcmp(a, "rotate"))
	{
		return;
	}
	a = exml_attribute_get(xml, "repeatCount");
	if (a)
	{
		if (!strcmp(a, "indefinity"))
			count = 0;
		else
			count = atoi(a);
	}
	ea = etch_animation_add(e->etch, ETCH_UINT32, _transform_rotate_uint32, o);
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	a = exml_attribute_get(xml, "from");
	etch_animation_keyframe_value_set(ek, atoi(a));
	etch_animation_keyframe_time_set(ek, 0, 0);
	a = exml_attribute_get(xml, "to");
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_value_set(ek, atoi(a));
	a = exml_attribute_get(xml, "dur");
	_time_parse(a, &sec, &msec);
	etch_animation_keyframe_time_set(ek, sec, msec);
	etch_animation_repeat_set(ea, count);
#endif
}

void _traverse_animate(Eix *e, Evas_Object *o)
{
	EXML *xml = e->xml;
	Etch_Animation *ea;
	Etch_Animation_Keyframe *ek;
	char *a;
	Etch_Animation_Callback cb;
	int sec, msec;
	int count = 1;

	a = exml_attribute_get(xml, "attributeName");
	if (!strcmp(a, "x"))
	{
		cb = _position_x_uint32;
	}
	else if (!strcmp(a, "y"))
	{
		cb = _position_y_uint32;
	}
	else
		return;
	a = exml_attribute_get(xml, "repeatCount");
	if (a)
	{
		if (!strcmp(a, "indefinity"))
			count = 0;
		else
			count = atoi(a);
	}
	ea = etch_animation_add(e->etch, ETCH_UINT32, cb, o);
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	a = exml_attribute_get(xml, "from");
	etch_animation_keyframe_value_set(ek, atoi(a));
	etch_animation_keyframe_time_set(ek, 0, 0);
	a = exml_attribute_get(xml, "to");
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_value_set(ek, atoi(a));
	a = exml_attribute_get(xml, "dur");
	_time_parse(a, &sec, &msec);
	etch_animation_keyframe_time_set(ek, sec, msec);
	etch_animation_repeat_set(ea, count);
}

void _traverse_rect(Eix *e)
{
	EXML *xml = e->xml;
	Evas_Object *o;
	char *tag;
	char *a1, *a2;
		
	/* parse the attributes */
	a1 = exml_attribute_get(xml, "w");
	a2 = exml_attribute_get(xml, "h");
	o = evas_object_rectangle_add(e->e);
	evas_object_resize(o, atoi(a1), atoi(a2));
	a1 = exml_attribute_get(xml, "x");
	a2 = exml_attribute_get(xml, "y");
	evas_object_move(o, atoi(a1), atoi(a2));
	
	/* traverse the childs */
	if ((tag = exml_down(xml)) == NULL)
	{
		printf("no more childs\n");
		return;
	}
	do
	{
		printf("%s\n", tag);
		if (!strcmp(tag, "animate"))
		{
			_traverse_animate(e, o);
		}
		//node = exml_get(xml);
		//printf("TAG=%s\n", tag);
	} while ((tag = exml_next_nomove(xml)) != NULL);
	exml_up(xml);
	evas_object_show(o);
}

void _traverse_img(Eix *e)
{
	EXML *xml = e->xml;
	Eix_Object *eo;
	Evas_Object *o;
	char *tag;
	char *a1, *a2;
	int imgw, imgh;
	
	eo = eix_object_add(e);
	/* parse the attributes */
	o = evas_object_image_add(e->e);
	a1 = exml_attribute_get(xml, "src");
	evas_object_image_file_set(o, a1, NULL);
	evas_object_image_size_get(o, &imgw, &imgh);
	a1 = exml_attribute_get(xml, "w");
	a2 = exml_attribute_get(xml, "h");
	evas_object_resize(o, atoi(a1), atoi(a2));
	evas_object_image_fill_set(o, 0, 0, atoi(a1), atoi(a2));
	a1 = exml_attribute_get(xml, "x");
	a2 = exml_attribute_get(xml, "y");
	evas_object_move(o, atoi(a1), atoi(a2));
	eo->o = o;
	/* traverse the childs */
	if ((tag = exml_down(xml)) == NULL)
	{
		printf("no more childs\n");
		return;
	}
	do
	{
		printf("%s\n", tag);
		if (!strcmp(tag, "animateTransform"))
		{
			_traverse_animate_transform(e, eo);
		}
		else if (!strcmp(tag, "animate"))
		{
			_traverse_animate(e, o);
		}
		//node = exml_get(xml);
		//printf("TAG=%s\n", tag);
	} while ((tag = exml_next_nomove(xml)) != NULL);
	exml_up(xml);
	evas_object_show(eo->o);
}

int _animator_cb(void *data)
{
	Eix *e = data;
	etch_timer_tick(e->etch);
	return 1;
}

void _traverse_eix(Eix *e)
{
	EXML *xml = e->xml;
	Evas_Object *o;
	char *tag;
	char *w, *h;
	
	/* parse the attributes */
	w = exml_attribute_get(xml, "w");
	h = exml_attribute_get(xml, "h");
	if (!w || !h)
		return;
	/* create the canvas */
	e->ee = ecore_evas_new("software_x11", 0, 0, atoi(w), atoi(h), NULL);
	ecore_evas_data_set(e->ee, "eix", e);
	/* callback for transformed objects */
	ecore_evas_callback_pre_render_set(e->ee, _pre_render);
	e->e = ecore_evas_get(e->ee);
	ecore_evas_show(e->ee);
	/* the background object */
	o = evas_object_rectangle_add(e->e);
	evas_object_color_set(o, 255, 255, 255, 255);
	evas_object_move(o, 0, 0);
	evas_object_resize(o, atoi(w), atoi(h));
	evas_object_show(o);
	/* create the etch */
	e->etch = etch_new();
	etch_timer_fps_set(e->etch, 30);
	ecore_timer_add((double)1/30, _animator_cb, e);
	/* parse the file */
	if ((tag = exml_down(xml)) == NULL)
	{
		printf("no more childs\n");
		return;
	}
	do
	{
		if (!strcmp(tag, "rect"))
		{
			_traverse_rect(e);
		}
		else if (!strcmp(tag, "img"))
		{
			_traverse_img(e);
		}
		
		//node = exml_get(xml);
		//printf("TAG=%s\n", tag);
	} while ((tag = exml_next(xml)) != NULL);
	exml_up(xml);
}

Eix_Object * eix_object_add(Eix *e)
{
	Eix_Object *eo;
	
	eo = calloc(1, sizeof(Eix_Object));
	e->objects = eina_inlist_append(e->objects, EINA_INLIST_GET(eo));

	return eo;
}

void eix_init(void)
{
	ecore_evas_init();
}
Eix * eix_new(void)
{
	Eix *e;
	e = calloc(1, sizeof(Eix));
	return e;
}

void eix_loop(void)
{
	ecore_main_loop_begin();
}

Eina_Bool eix_file_load(Eix *e, const char *f)
{
	EXML *xml;
	EXML_Node *node;
	
	xml = exml_new();
	exml_file_read(xml, f);
	node = exml_get(xml);
	if (strcmp(node->tag, "eix"))
	{
		return EINA_FALSE;
	}
	else
	{
		e->xml = xml;
		_traverse_eix(e);
		return EINA_TRUE;
	}
}


int main(int argc, char **argv)
{
	Eix *e;
	
	/* load the file */
	if (argv[1])
	{
		printf("parsing file %s\n", argv[1]);
	}
	eix_init();
	e = eix_new();
	eix_file_load(e, argv[1]);
	eix_loop();
	return 1;
}
