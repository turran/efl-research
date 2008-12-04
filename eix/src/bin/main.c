#include <string.h>

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
} Eix;

typedef struct _Eix_Object
{
	Evas_Object *o;
	Enesim_Matrix *m;
	Enesim_Surface *src;
	Enesim_Surface *dst;
	Eina_Bool render;
	Enesim_Transformation *tx;
} Eix_Object;

Eix_Object * eix_object_add(Eix *e);
void eix_init(void);
Eix * eix_new(void);
void eix_loop(void);
Eina_Bool eix_file_load(Eix *e, const char *f);

void _pre_render(Ecore_Evas *ee)
{
	/* iterate over the list of objects */
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
void _position_x_uint32(void *odata, void *pdata)
{
	Evas_Object *o = odata;
	int x = *(unsigned int *)pdata;
	int y;
	
	evas_object_geometry_get(o, NULL, &y, NULL, NULL);
	//printf("moving to %d %d\n", x, y);
	evas_object_move(o, x, y);
}

void _position_y_uint32(void *odata, void *pdata)
{
	Evas_Object *o = odata;
	int y = *(unsigned int *)pdata;
	int x;
		
	evas_object_geometry_get(o, &x, NULL, NULL, NULL);
	evas_object_move(o, x, y);
}

void _transform_uint32(void *odata, void *pdata)
{
	Eix_Object *o = odata;
	Eina_Rectangle rect;
	Enesim_Surface_Data data;
	Enesim_Matrix *matrix;
	int imgw, imgh;
	int deg = *(unsigned int *)pdata;

#if 1
	enesim_matrix_rotate(o->m, (float)deg*(3.1416)/180);
	enesim_surface_data_get(o->dst, &data);
	enesim_surface_size_get(o->dst, &imgw, &imgh);
	memset(data.data.argb8888.plane0, 0x0, imgw * imgh * 4);
	eina_rectangle_coords_from(&rect, 0, 0, imgw, imgh);
	//printf("%d %d\n", imgh/2, imgw/2);
	//enesim_transformation_origin_set(o->tx, 0, 0);
	enesim_transformation_origin_set(o->tx, imgw/2, imgh/2);
#if 1
	matrix = enesim_matrix_new();
	enesim_matrix_translate(matrix, -imgw/2, -imgh/2);
	enesim_matrix_compose(o->m, matrix, o->m);
	enesim_matrix_delete(matrix);
#endif
	enesim_transformation_matrix_set(o->tx, o->m);
	enesim_transformation_apply(o->tx, o->src, &rect, o->dst, &rect);
	evas_object_image_pixels_dirty_set(o->o, 1);
	
#endif
}

/* Simple object class to test all properties */
Etch_Object_Property oproperties[] = {
	{
		.type = ETCH_POSITION_X_UINT32,
		.set = &_position_x_uint32, 
	},
	{
		.type = ETCH_POSITION_Y_UINT32,
		.set = &_position_y_uint32,
	},
	{
		.type = (ETCH_USER << 16) | (ETCH_UINT32),
		.set = &_transform_uint32,
	},
};

Etch_Object_Class oclass = {
	.props = oproperties,
};

void _time_parse(const char *s, int *sec, int *msec)
{
	//int len = strlen(s);
	char *end;
	int total;
	
	total = strtol(s, &end, 10);
	printf("total = %d\n", total);
	*sec = total;
	
}

void _traverse_animate_transform(Eix *e, Eix_Object *o)
{
	EXML *xml = e->xml;
	Etch_Animation *ea;
	Etch_Object *eo;
	Etch_Animation_Keyframe *ek;
	char *a;
	int prop;
	int sec, msec;
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
	if (!strcmp(a, "rotate"))
	{
		prop = ETCH_USER;		
	}
	else
		return;
	ea = etch_animation_new(ETCH_UINT32);
	eo = etch_object_add(e->etch, &oclass, "object2", o);
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
	etch_object_animation_set(eo, prop, ea);
#endif
}

void _traverse_animate(Eix *e, Evas_Object *o)
{
	EXML *xml = e->xml;
	Etch_Animation *ea;
	Etch_Object *eo;
	Etch_Animation_Keyframe *ek;
	char *a;
	int prop;
	int sec, msec;
	
	a = exml_attribute_get(xml, "attributeName");
	if (!strcmp(a, "x"))
	{
		
		prop = ETCH_POSITION_X;		
	}
	else if (!strcmp(a, "y"))
	{
		prop = ETCH_POSITION_Y;
	}
	else
		return;
	ea = etch_animation_new(ETCH_UINT32);
	eo = etch_object_add(e->etch, &oclass, "object1", o);
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
	etch_object_animation_set(eo, prop, ea);
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
	
	printf("ADDED\n");
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
	evas_object_color_set(o, 255, 0, 0, 255);
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
	return eo;
}

void eix_init(void)
{
	ecore_evas_init();
}
Eix * eix_new(void)
{
	Eix *e;
	e = malloc(sizeof(Eix));
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
