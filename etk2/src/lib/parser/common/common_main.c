/*
 * common_main.c
 *
 *  Created on: 15-jul-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool eon_parser_coord_str_from(Eon_Coord *c, char *v)
{
	Eon_Coord_Type ctype = EON_COORD_ABSOLUTE;
	int i;
	char *rel;

	rel = strchr(v, '%');
	if (rel)
	{
		rel = '\0';
		ctype = EON_COORD_RELATIVE;
	}
	i = strtol(v, NULL, 10);
	eon_coord_set(c, i, ctype);
	return EINA_TRUE;
}

Eina_Bool eon_parser_matrix_str_from(Enesim_Matrix *m, char *v)
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

Eina_Bool eon_parser_color_str_from(Eon_Color *c, char *v)
{
	if (!strcmp(v, "white"))
		eon_color_set(c, 0xff, 0xff, 0xff, 0xff);
	else if (!strcmp(v, "red"))
		eon_color_set(c, 0xff, 0xff, 0x00, 0x00);
	else if (!strcmp(v, "blue"))
		eon_color_set(c, 0xff, 0x00, 0x00, 0xff);
	else
	{
		/* hex | dec */
		*c = strtoul(v, NULL, 0);
	}
	return EINA_TRUE;
}

Eina_Bool eon_parser_rop_str_from(Ekeko_Value *ev, char *v)
{
	Enesim_Rop rop = ENESIM_BLEND;

	if (!strcmp(v, "blend"))
		rop = ENESIM_BLEND;
	else if (!strcmp(v, "fill"))
		rop = ENESIM_FILL;
	ekeko_value_int_from(&ev, rop);

	return EINA_TRUE;
}

Eina_Bool eon_parser_clock_str_from(Eon_Clock *c, char *v)
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
		num = strtof(v, NULL);
		dec = num - (int)num;
		c->seconds = (int)num;
		c->micro = dec * 1000000;
		printf("%ld %ld\n", c->seconds, c->micro);
	}
	else
	{
		num = atof(v);
		dec = num - (int)num;
		c->seconds = (int)num;
		c->micro = dec * 100000;
	}
	return EINA_TRUE;
}

Ekeko_Object * eon_parser_object_new(Ekeko_Object *p, Eon_Type_Constructor ctr)
{
	Ekeko_Object *o;

	o = ekeko_type_instance_new(ctr());

	if (!p)
		return o;

	if (ekeko_object_child_append(o, p))
		return o;
	else
	{
		ekeko_type_instance_delete(o);
		return NULL;
	}
}

Ekeko_Object * eon_parser_shape_new(Ekeko_Object *p, Eon_Type_Constructor ctr)
{
	Ekeko_Object *o;

	o = eon_parser_object_new(p, eon_polygon_type_get);
	/* default atributres */
	if (o)
	{
		eon_shape_rop_set((Eon_Shape *)o, ENESIM_BLEND);
	}
}

Ekeko_Object * eon_parser_polygon_new(Ekeko_Object *p)
{
	return eon_parser_object_new(p, eon_polygon_type_get);
}

Ekeko_Object * eon_parser_rect_new(Ekeko_Object *p)
{
	return eon_parser_object_new(p, eon_rect_type_get);
}

Ekeko_Object * eon_parser_circle_new(Ekeko_Object *p)
{
	return eon_parser_object_new(p, eon_circle_type_get);
}
