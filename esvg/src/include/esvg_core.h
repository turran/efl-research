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
	float value;
	ESVG_Length_Type type;
} ESVG_Length, ESVG_Coord;

//typedef ESVG_Length ESVG_Angle;

static inline Eina_Bool eina_length_type_is_relative(ESVG_Length_Type type)
{
	if ((type == ESVG_LENGTH_TYPE_PERCENTAGE) ||
		(type == ESVG_LENGTH_TYPE_EMS))
		return EINA_TRUE;
	return EINA_FALSE;
}

EAPI Eina_Bool esvg_length_get(const char *str, ESVG_Length *length);


#endif /*ESVG_CORE_H_*/
