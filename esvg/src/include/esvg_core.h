#ifndef ESVG_CORE_H_
#define ESVG_CORE_H_

typedef unsigned int ESVG_Color;
typedef float ESVG_Length_Value;
typedef ESVG_Length_Value ESVG_Coord_Value;

/* basic types */
typedef enum _ESVG_Length_Type
{
	ESVG_LENGTH_TYPE_UNKNOWN,
	ESVG_LENGTH_TYPE_NUMBER,
	ESVG_LENGTH_TYPE_PERCENTAGE,
	ESVG_LENGTH_TYPE_EMS,
	ESVG_LENGTH_TYPE_EXS,
	ESVG_LENGTH_TYPE_PX,
	ESVG_LENGTH_TYPE_CM,
	ESVG_LENGTH_TYPE_MM,
	ESVG_LENGTH_TYPE_IN,
	ESVG_LENGTH_TYPE_PT,
	ESVG_LENGTH_TYPE_PC
} ESVG_Length_Type;

typedef struct _ESVG_Length
{
	float value; /* normalized value, read only */
	float type_value; /* value in type units */
	ESVG_Length_Type type;
} ESVG_Length, ESVG_Coord;

//typedef ESVG_Length ESVG_Angle;


typedef enum _ESVG_Fill_Rule
{
	ESVG_FILL_RULE_NONZERO,
	ESVG_FILL_RULE_EVENODD,
} ESVG_Fill_Rule;

typedef enum _ESVG_Stroke_Line_Cap
{
	ESVG_STROKE_LINE_CAP_BUTT,
	ESVG_STROKE_LINE_CAP_ROUND,
	ESVG_STROKE_LINE_CAP_SQUARE,
} ESVG_Stroke_Line_Cap;

typedef enum _ESVG_Stroke_Line_Join
{
	ESVG_STROKE_LINE_JOIN_MITER,
	ESVG_STROKE_LINE_JOIN_ROUND,
	ESVG_STROKE_LINE_JOIN_BEVEL,
} ESVG_Stroke_Line_Join;

typedef enum _ESVG_Interpolation
{
	ESVG_INTERPOLATION_AUTO,
	ESVG_INTERPOLATION_SRGB,
	ESVG_INTERPOLATION_LINEAR_RGB,
} ESVG_Interpolation;


/* id
 * xml:base
 * xml:lang
 * xml:space
 */
typedef struct _ESVG_Attribute_Core
{
	char *id;
} ESVG_Attribute_Core;


typedef struct _ESVG_Attribute_Paint
{
	ESVG_Color color;
	ESVG_Color fill;
	ESVG_Fill_Rule fill_rule;
	ESVG_Color stroke;
	char stroke_dash_array;
	ESVG_Length stroke_dashoffset;
	ESVG_Stroke_Line_Cap stroke_linecap;
	ESVG_Stroke_Line_Join stroke_linejoin;	
	double stroke_miterlimit; /* >= 1 */
	ESVG_Length stroke_width;
	ESVG_Interpolation color_interpolation;
	ESVG_Interpolation color_interpolation_rendering;
} ESVG_Attribute_Paint;

typedef struct _ESVG_Attribute_Opacity
{
	double opacity; /* 0.0 -> 1.0 */
	double stroke_opacity; /* 0.0 -> 1.0 */
	double fill_opacity; /* 0.0 -> 1.0 */
} ESVG_Attribute_Opacity;

/* display
 * image-rendering
 * pointer-events
 * shape-rendering
 * text-rendering
 * visibility
 */
typedef struct _ESVG_Attribute_Graphics
{
	
} ESVG_Attribute_Graphics;

/* clip-path
 * clip-rule
 */
typedef struct _ESVG_Attribute_Clip
{
	
} ESVG_Attribute_Clip;

/* mask
 */
typedef struct _ESVG_Attribute_Mask
{
	
} ESVG_Attribute_Mask;

/*  Filter.attrib  	filter
FilterColor.attrib 	color-interpolation-filters
FilterPrimitive.attrib 	x, y, width, height, result
FilterPrimitiveWithIn.attrib 	FilterPrimitive.attrib, in
*/

/* cursor */
typedef struct _ESVG_Attribute_Cursor
{
	
} ESVG_Attribute_Cursor;

static inline Eina_Bool esvg_length_type_is_relative(ESVG_Length_Type type)
{
	if ((type == ESVG_LENGTH_TYPE_PERCENTAGE) ||
		(type == ESVG_LENGTH_TYPE_EMS))
		return EINA_TRUE;
	return EINA_FALSE;
}

EAPI Eina_Bool esvg_length_get(const char *str, ESVG_Length *length);

#endif /*ESVG_CORE_H_*/
