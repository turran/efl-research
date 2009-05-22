//#define _XOPEN_SOURCE 600
//#define _ISOC99_SOURCE

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdarg.h>

#include "Etk2.h"
#include "EXML.h"

/*
 * The testsuite is an application that parses xml files with etk scenes
 * TODO make etk2 type system to be defined at etk_init() to avoid strcmp => etk_foo
 * attributes of type VALUE are relative to another attribute, how to handle it?
 *
 */
char *engine = "sdl";
char *image_dir = "./";

void parse(EXML *exml, Ekeko_Object *doc);

void help(void)
{
	printf("etk2_testsuite [OPTIONS] FILE\n");
	printf("-h This help\n");
	printf("-e Engine\n");
	printf("-W Width\n");
	printf("-H Height\n");
	printf("-I Image directory\n");
	printf("FILE Etk2 XML file\n");
}


void function_parse(char *v, int *num, ...)
{
	va_list ap;
	char *tmp;
	char *end;

	*num = 0;
	tmp = v;
	if (*tmp == '(')
		tmp++;
	va_start(ap, num);
	do
	{
		float *f;
		float ftmp;

		/* get the value */
		ftmp = strtof(tmp, &end);
		if (end == tmp)
			break;
		if (*end)
		{
			printf("end = %c\n", *end);
			tmp = end + 1;
		}
		else
			break;
		f = va_arg(ap, float *);
		if (!f)
		{
			va_end(ap);
			return;
		}
		*f = ftmp;
		(*num)++;
	} while (tmp);
	va_end(ap);
}

Eina_Bool matrix_parse(Enesim_Matrix *m, char *v)
{
	/* well known names */
	/* rotate(degrees) */
	if (!strncmp(v, "rotate", strlen("rotate")))
	{
		float grad;
		int num;

		function_parse(v + strlen("rotate"), &num, &grad);
		if (num != 1)
		{
			/* TODO put some warnings */
			return EINA_FALSE;
		}
		enesim_matrix_rotate(m, grad);
		return EINA_TRUE;
	}
	/* move(x, y) */
	else if (!strncmp(v, "move", strlen("move")))
	{
		float tx, ty;
		int num;

		function_parse(v + strlen("move"), &num, &tx, &ty);
		if (num != 2)
		{
			/* TODO put some warnings */
			return EINA_FALSE;
		}
		enesim_matrix_translate(m, tx, ty);
		return EINA_TRUE;

	}
	/* scale(s) scale(sx, sy) */
	else if (!strncmp(v, "scale", strlen("scale")))
	{
		float sx, sy;
		int num;

		function_parse(v + strlen("scale"), &num, &sx, &sy);
		if ((num < 1) || (num > 2))
		{
			/* TODO put some warnings */
			return EINA_FALSE;
		}
		if (num == 1)
			sy = sx;
		printf("Scaling by %g %g\n", sx, sy);
		enesim_matrix_scale(m, sx, sy);
		return EINA_TRUE;
	}
	/* matrix="xx xy xz yx yy yz zx zy zz */
	else
	{
		float matrix[9];
		int i;
		char *tmp, *end;

		tmp = v;
		for (i = 0; i < 9; i++)
		{
			/* get the value */
			matrix[i] = strtof(tmp, &end);
			if (end == tmp)
				break;
			if (*end)
			{
				printf("%g end = %c\n", matrix[i], *end);
				tmp = end + 1;
			}
			else
				break;
		}
		if (i < 8)
			return EINA_FALSE;
		enesim_matrix_values_set(m, matrix[0], matrix[1], matrix[2],
				matrix[3], matrix[4], matrix[5], matrix[6],
				matrix[7], matrix[8]);
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

void clock_parse(Etk_Clock *c, char *v)
{
	float num = 0;
	char *units;
	float dec;

	units = v + strlen(v) - 1;

	/* minutes */
	if (*units == 'm')
	{
		float secs;

		*units = '\0';
		num = atof(v);
		secs = num * 60;

		dec = secs - (int)secs;
		c->seconds = (int)secs;
		c->micro = dec * 100000;
	}
	/* seconds  | no units */
	else if (*units == 's')
	{

		*units = '\0';
		//num = strtof(v, NULL);
		num = atof(v);
		printf("num = %g\n", num);
		dec = num - (int)num;
		c->seconds = (int)num;
		c->micro = dec * 100000;
	}
	else
	{
		num = atof(v);
		dec = num - (int)num;
		c->seconds = (int)num;
		c->micro = dec * 100000;
	}
}

/* parsing of the different attributes */
void object_etk_attribute_set(Ekeko_Object *o, Ekeko_Value_Type type, char *attr, char *value)
{
	Ekeko_Value v;

	if (type == ETK_PROPERTY_COORD)
	{
		Etk_Coord c;
		Etk_Coord_Type ctype = ETK_COORD_ABSOLUTE;
		int i;
		char *rel;

		rel = strchr(value, '%');
		if (rel)
		{
			rel = '\0';
			ctype = ETK_COORD_RELATIVE;
		}
		i = strtol(value, NULL, 10);
		etk_coord_set(&c, i, ctype);
		etk_value_coord_from(&v, &c);
		ekeko_object_property_value_set(o, attr, &v);
	}
	else if (type == ETK_PROPERTY_CLOCK)
	{
		Etk_Clock c;
		char *tmp;

		tmp = strdup(value);
		clock_parse(&c, tmp);
		etk_value_clock_from(&v, &c);
		ekeko_object_property_value_set(o, attr, &v);
		free(tmp);
	}
	else if (type == ETK_PROPERTY_TRIGGER)
	{
		Etk_Trigger t;

		t.event = value;
		/* FIXME the object can be referenced by #id :) */
		t.obj = ekeko_object_parent_get(o);
		etk_value_trigger_from(&v, &t);
		ekeko_object_property_value_set(o, attr, &v);
	}
	else if (type == ETK_PROPERTY_MATRIX)
	{
		Enesim_Matrix m;

		if (matrix_parse(&m, value))
		{
			etk_value_matrix_from(&v, &m);
			ekeko_object_property_value_set(o, attr, &v);
		}
	}
	else if (type == ETK_PROPERTY_COLOR)
	{
		Etk_Color c = 0;

		if (!strcmp(value, "white"))
			etk_color_set(&c, 0xff, 0xff, 0xff, 0xff);
		else if (!strcmp(value, "red"))
			etk_color_set(&c, 0xff, 0xff, 0x00, 0x00);
		else if (!strcmp(value, "blue"))
			etk_color_set(&c, 0xff, 0x00, 0x00, 0xff);
		else
		{
			/* hex | dec */
			c = strtoul(value, NULL, 0);
		}
		etk_value_color_from(&v, c);
		ekeko_object_property_value_set(o, attr, &v);
	}
}


void object_value_attribute_set(Ekeko_Object *o, Ekeko_Value_Type type, char *attr, char *name)
{
	/* if it is an animation get the prop name */
}

void object_attribute_set(Ekeko_Object *o, Ekeko_Value_Type type, char *attr, char *name)
{
	Ekeko_Value v;

	/* properties with special values */
	/* rop */
	if (!strcmp(attr, "rop"))
	{
		Enesim_Rop rop = ENESIM_BLEND;

		if (!strcmp(name, "blend"))
			rop = ENESIM_BLEND;
		else if (!strcmp(name, "fill"))
			rop = ENESIM_FILL;
		ekeko_value_int_from(&v, rop);
		ekeko_object_property_value_set(o, attr, &v);
		return;
	}
	/* quality */
	/* properties with simple values */
	switch (type)
	{
		case PROPERTY_INT:
		ekeko_value_int_from(&v, strtol(name, NULL, 0));
		ekeko_object_property_value_set(o, attr, &v);
		break;

		case PROPERTY_VALUE:
		{
			/* FIXME remove "Etk_Animation" */
			if (ekeko_type_instance_is_of(o, "Etk_Animation"))
			{
				Ekeko_Value nvalue;
				Ekeko_Object *parent;
				Ekeko_Value_Type vtype;

				/* get the name of the attribute to animate */
				/* FIXME remove "name" */
				ekeko_object_property_value_get(o, "name", &nvalue);
				parent = ekeko_object_parent_get(o);
				if (!parent)
					return;

				if (ekeko_type_instance_is_of(o, "Etk_Animation_Basic"))
				{
					Property *prop;

					/* in case of anim tag, handle every type */
					prop = ekeko_object_property_get(parent, nvalue.value.string_value);
					vtype = ekeko_property_value_type_get(prop);
				}
				else if (ekeko_type_instance_is_of(o, "Etk_Animation_Matrix"))
				{
					/* in case of animmatrix, this is handled differently */
					printf("HEEEEEEEEEEEEEEEEEEEEREEEEEEE %s %s\n", name, attr);
					vtype = PROPERTY_INT;
				}
				printf("[PARSER] Going to set a value for prop %d %s with %d\n", nvalue.type, nvalue.value.string_value, vtype);
				object_attribute_set(o, vtype, attr, name);
			}
		}
		return;

		case PROPERTY_BOOL:
		case PROPERTY_CHAR:
		case PROPERTY_FLOAT:
		case PROPERTY_DOUBLE:
		case PROPERTY_SHORT:
		case PROPERTY_LONG:
		case PROPERTY_RECTANGLE:
		case PROPERTY_POINTER:
		case PROPERTY_OBJECT:
		return;

		case PROPERTY_STRING:
		{
			/* FIXME remove "Etk_Image" */
			if (ekeko_type_instance_is_of(o, "Etk_Image") && !strcmp(attr, "file"))
			{
				char real_file[256];

				sprintf(real_file, "%s/%s", image_dir, name);
				printf("image at %s\n", real_file);
				ekeko_value_str_from(&v, strdup(real_file));
				ekeko_object_property_value_set(o, attr, &v);
			}
			else
			{
				ekeko_value_str_from(&v, strdup(name));
				ekeko_object_property_value_set(o, attr, &v);
			}
		}
		break;

		default:
		object_etk_attribute_set(o, type, attr, name);
		return;
	}
}

void attributes_set(void *value, void *data)
{
	Ecore_Hash_Node *n = value;
	Ekeko_Object *o = data;
	Property *prop;
	Ekeko_Value_Type type;

	//printf("[PARSER] Setting attribute %s\n", (char *)n->key);
	prop = ekeko_object_property_get(o, n->key);
	if (!prop)
		return;
	/* in case the object is an animation skip the prop attribute
	 * as it is already set
	 */
	/* FIXME remove "Etk_Animation" */
	if (ekeko_type_instance_is_of(o, "Etk_Animation"))
	{
		if (ekeko_property_id_get(prop) == ETK_ANIMATION_PROPERTY)
		{
			printf("[PARSER] Skipping the name attribute\n");
			return;
		}
	}

	type = ekeko_property_value_type_get(prop);
	object_attribute_set(o, type, n->key, n->value);
}

Ekeko_Object * tag_create(char *tag, EXML *exml, Ekeko_Object *parent)
{
	EXML_Node *n;
	Ekeko_Object *o = NULL;

	n = exml_get(exml);
	printf("[PARSER] creating tag %s for parent %s\n", tag, ekeko_object_type_name_get(parent));
	if (!strcmp(tag, "etk"))
	{
		o = (Ekeko_Object *)etk_canvas_new((Etk_Canvas *)parent);
		etk_rect_show(o);
		etk_rect_rop_set(o, ENESIM_BLEND);
	}
	if (!strcmp(tag, "rect"))
	{
		o = (Ekeko_Object *)etk_rect_new((Etk_Canvas *)parent);
		etk_rect_show(o);
		etk_rect_rop_set(o, ENESIM_BLEND);
	}
	else if (!strcmp(tag, "image"))
	{
		o = (Ekeko_Object *)etk_image_new((Etk_Canvas *)parent);
		etk_rect_show(o);
		etk_rect_rop_set(o, ENESIM_BLEND);
	}
	else if (!strcmp(tag, "anim") || !strcmp(tag, "animMatrix"))
	{
		char *value;

		if (!strcmp(tag, "animMatrix"))
			o = (Ekeko_Object *)etk_animation_matrix_new();
		else
			o = (Ekeko_Object *)etk_animation_basic_new();
		if (!o)
			return NULL;
		ekeko_object_child_append(parent, o);
		/* if we set the attributes "from/to" we should know the prop name
		 * before setting them
		 */
		/* FIXME remove "name" */
		value = ecore_hash_get(n->attributes, "name");
		if (!value)
		{
			return NULL;
		}
		object_attribute_set(o, PROPERTY_STRING, "name", value);
	}
	if (!o)
		return NULL;
	/* attributes */
	ecore_hash_for_each_node(n->attributes, attributes_set, o);
	/* TODO if is renderable show() it */
	return o;
}

void parse(EXML *exml, Ekeko_Object *parent)
{
	EXML_Node *n;
	char *tag;

	if (!parent)
		return;
	printf("[PARSER] parsing %s\n", ekeko_object_type_name_get(parent));
	n = exml_get(exml);
	tag = exml_down(exml);
	while (tag)
	{
		Ekeko_Object *o;

		o = tag_create(tag, exml, parent);
		/* children */
		parse(exml, o);
		/* siblings */
		tag = exml_next(exml);
	}
	//printf("going up\n");
	exml_goto_node(exml, n);
}

int main(int argc, char **argv)
{
	EXML *exml;
	EXML_Node *n;
	Etk_Document *doc;
	Etk_Canvas *canvas;
	int w = 320;
	int h = 240;

	char *short_options = "I:e:hW:H:";
	struct option long_options[] = {
		{"image", 1, 0, 'I'},
		{"engine", 1, 0, 'e'},
		{"help", 0, 0, 'h'},
		{"width", 1, 0, 'W'},
		{"height", 1, 0, 'H'},
		{0, 0, 0, 0}
	};
	int option;
	char c;
	char *file = argv[argc - 1];

	while ((c = getopt_long(argc, argv, short_options, long_options,
			&option)) != -1)
	{
		switch (c)
		{
			case 'h':
			help();
			return 0;

			case 'W':
			w = atoi(optarg);
			break;

			case 'I':
			image_dir = strdup(optarg);
			break;

			case 'H':
			h = atoi(optarg);
			break;

			default:
			help();
			return 0;
			break;
		}
	}
	exml = exml_new();
	printf("%s\n", file);
	if (!exml_file_read(exml, file))
	{
		printf("no file\n");
		return -1;
	}
	n = exml_get(exml);
	if (!n || strcmp(n->tag, "etk"))
	{
		printf("no etk file\n");
		return -2;
	}
	/* create the context */
	etk_init();
	doc = etk_document_new(engine, w, h);
	/* create the canvas */
	canvas = etk_document_canvas_get(doc);
	ecore_hash_for_each_node(n->attributes, attributes_set, canvas);
	/* parse the file */
	parse(exml, (Ekeko_Object *)canvas);
	ekeko_object_dump((Ekeko_Object *)doc, ekeko_object_dump_printf);
	etk_loop();
	etk_shutdown();
	exml_destroy(exml);

	return 0;
}
