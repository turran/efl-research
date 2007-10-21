#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "Enesim.h"
#include "EXML.h"

typedef struct _SVG_Context
{
	DATA32 color;
} SVG_Context;

static SVG_Context * _context_save(SVG_Context *c)
{

}

static void _context_restore(SVG_Context *c, SVG_Context *cb)
{

}

static void _context_color_set(SVG_Context *c, DATA32 col)
{

}


typedef struct _SVG_Document
{
	SVG_Context ct;
	Enesim_Surface *dst;
} SVG_Document;

/*
 * "1pt" equals "1.25px" (and therefore 1.25 user units)
 * "1pc" equals "15px" (and therefore 15 user units)
 * "1mm" would be "3.543307px" (3.543307 user units)
 * "1cm" equals "35.43307px" (and therefore 35.43307 user units)
 * "1in" equals "90px" (and therefore 90 user units)
 */
static float _units_get(const char *str)
{
	char units[3];
	float c;
	float r;

	assert(str);
	sscanf(str, "%f%s", &c, units);
	if (!strcmp(units, "cm"))
	{
		r = 35.43307 * c; 	
	}
	else if (!strcmp(units, "pt"))
	{
		r = 1.25 * c;
	}
	return r;
}

static DATA32 _paint_color_get(char *str)
{
	DATA32 color;

	sscanf(str + 1, "%x", &color);
	/* FIXME for now opaque */
	color |= 0xff000000;
	return color;
}

static int _points_point_get(char **str, float *x, float *y)
{
	int n;

	sscanf(*str, "%f,%f %n", x, y, &n);
	*str = *str + n;
	if (!*str || !strlen(*str))
		return 0;
	else
		return 1;
}

static int _document_get(SVG_Document *d, EXML_Node *n)
{
	float w, h;
	
	assert(d);
	assert(n);
	
	w = _units_get(ecore_hash_get(n->attributes, "width"));
	h = _units_get(ecore_hash_get(n->attributes, "height"));
	d->dst = surface_new((int)ceilf(w), (int)ceilf(h), ENESIM_SURFACE_ARGB8888);
}

/* polygons */
static void _polygon_parse(SVG_Document *d, EXML *xml)
{
	char *data;
	DATA32 old_color;
	/* copy context */
	old_color = d->ct.color;
	/* parse attributes */
	if ((data = exml_attribute_get(xml, "fill")) != NULL)
	{
		//printf("P: FILL %s\n", data);
		d->ct.color = _paint_color_get(data);
	}
	if ((data = exml_attribute_get(xml, "transform")) != NULL)
	{
		//printf("P: TRANSFORM %s\n", data);
	}
	if ((data = exml_attribute_get(xml, "points")) != NULL)
	{
		//Enesim_Component *path;
		Enesim_Rasterizer *rs;
		Enesim_Scanline *sl;
		Enesim_Renderer *rd;
		float x, y;
		int ret;
		
		//path = enesim_path_new();
		rs = enesim_rasterizer_new();
		ret = _points_point_get(&data, &x, &y);
		enesim_rasterizer_vertex_add(rs, x, y);
		//enesim_path_move_to(path, x, y);
		while (ret)
		{
			ret = _points_point_get(&data, &x, &y);
			enesim_rasterizer_vertex_add(rs, x, y);
			//enesim_path_line_to(path, x, y);
		}
		sl = enesim_scanline_alias_new();
		enesim_rasterizer_generate(rs, sl);
		rd = enesim_fill_color_new();
		enesim_fill_color_color_set(rd, d->ct.color);
		enesim_renderer_draw(rd, sl, d->dst);
		enesim_scanline_delete(sl);
	}
	/* restore context */
	d->ct.color = old_color;
}

/* groups */
static void _group_traverse(SVG_Document *d, EXML *xml);
static void _group_parse(SVG_Document *d, EXML *xml)
{
	char *data;
	DATA32 old_color;
	/* copy context */
	old_color = d->ct.color;
	/* parse attributes */
	if ((data = exml_attribute_get(xml, "fill")) != NULL)
	{
		printf("G: FILL\n");
		d->ct.color = _paint_color_get(data);
	}
	if ((data = exml_attribute_get(xml, "transform")) != NULL)
	{
		printf("G: TRANSFORM\n");
	}
	/* parse childs */
	_group_traverse(d, xml);
	/* restore context */
	d->ct.color = old_color;
}

static void _group_traverse(SVG_Document *d, EXML *xml)
{
	char *tag;

	if ((tag = exml_down(xml)) == NULL)
	{
		return;
	}
	/* parse tags */
	do
	{
		if (!strcmp(tag, "g"))
		{
			_group_parse(d, xml);
		}
		else if (!strcmp(tag, "polygon"))
		{
			_polygon_parse(d, xml);
		}
		else
			printf("TAG = %s\n", tag);
	} while ((tag = exml_next_nomove(xml)) != NULL);
	exml_up(xml);
}


int main(int argc, char **argv)
{
	EXML *xml;
	EXML_Node *n;
	SVG_Document d;
	char *tag;

	xml = exml_new();
	if (!exml_file_read(xml, "../data/lion.svg"))
	{
		printf("cant read file??\n");
	}
	n = exml_get(xml);
	if (strcmp(n->tag, "svg"))
	{
		printf("no svg\n");
		return -1;
	}
	_document_get(&d, n);
	if ((tag = exml_down(xml)) == NULL)
	{
		printf("empty tags\n");
		return -2;
	}
	/* parse tags */
	do
	{
		if (!strcmp(tag, "g"))
		{
			_group_parse(&d, xml);
		}
		else
			printf("TAG = %s\n", tag);
	} while ((tag = exml_next_nomove(xml)) != NULL);
	png_save(d.dst, "/tmp/enesim2.png", 0);
	return 0;
}
