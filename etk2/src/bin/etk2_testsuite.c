#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <getopt.h>

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
		num = strtof(v, NULL);
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
		num = strtof(v, NULL);
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

		clock_parse(&c, value);
		etk_value_clock_from(&v, &c);
		ekeko_object_property_value_set(o, attr, &v);
	}
	else if (type == ETK_PROPERTY_TRIGGER)
	{
		Etk_Trigger t;

		t.event = value;
		t.obj = o;
		etk_value_trigger_from(&v, &t);
		ekeko_object_property_value_set(o, attr, &v);
	}
	else if (type == ETK_PROPERTY_MATRIX)
	{

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
	switch (type)
	{
		case PROPERTY_INT:
		ekeko_value_int_from(&v, strtol(name, NULL, 0));
		break;

		case PROPERTY_VALUE:
		{
			/* FIXME remove "Etk_Animation" */
			if (ekeko_type_instance_is_of(o, "Etk_Animation"))
			{
				Ekeko_Value nvalue;
				Ekeko_Object *parent;
				Property *prop;
				Ekeko_Value_Type vtype;

				/* get the name of the attribute to animate */
				/* FIXME remove "name" */
				ekeko_object_property_value_get(o, "name", &nvalue);
				parent = ekeko_object_parent_get(o);
				prop = ekeko_object_property_get(parent, nvalue.value.string_value);
				vtype = ekeko_property_value_type_get(prop);
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
			}
			else
				ekeko_value_str_from(&v, strdup(name));
		}
		break;

		default:
		object_etk_attribute_set(o, type, attr, name);
		return;
	}
	ekeko_object_property_value_set(o, attr, &v);
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
	if (!strcmp(tag, "etk"))
	{

	}
	if (!strcmp(tag, "rect"))
	{
		o = (Ekeko_Object *)etk_rect_new((Etk_Canvas *)parent);
		etk_rect_show(o);
		etk_rect_rop_set(o, ENESIM_FILL);
	}
	else if (!strcmp(tag, "image"))
	{
		o = (Ekeko_Object *)etk_image_new((Etk_Canvas *)parent);
		etk_rect_show(o);
		etk_rect_rop_set(o, ENESIM_BLEND);
	}
	else if (!strcmp(tag, "anim"))
	{
		char *value;

		o = (Ekeko_Object *)etk_animation_new();
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
	n = exml_get(exml);
	tag = exml_down(exml);
	//printf("going down\n");
	while (tag)
	{
		Ekeko_Object *o;

		o = tag_create(tag, exml, parent);
		/* children */
		parse(exml, o);
		/* siblings */
		//printf("going next\n");
		tag = exml_next(exml);
	}
	exml_goto_node(exml, n);
	//printf("going up\n");
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
