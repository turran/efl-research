/*
 * etk_value.h
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */

#ifndef ETK_VALUE_H_
#define ETK_VALUE_H_

extern int ETK_PROPERTY_COORD;

typedef enum
{
	ETK_COORD_ABSOLUTE,
	ETK_COORD_RELATIVE,
} Etk_Coord_Type;

typedef struct _Etk_Coord
{
	int final;
	Etk_Coord_Type type;
	int value;
} Etk_Coord;

static inline void etk_value_coord_from(Value *v, Etk_Coord *coord, int value, Etk_Coord_Type type)
{
	coord->value = value;
	coord->type = type;
	v->value.pointer_value = coord;
}

#endif /* ETK_VALUE_H_ */
