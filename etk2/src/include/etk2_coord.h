#ifndef ETK2_COORD_H_
#define ETK2_COORD_H_

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

static inline void etk_coord_set(Etk_Coord *c, int value, Etk_Coord_Type type)
{
	c->value = value;
	c->type = type;
}

#endif /* ETK2_COORD_H_ */
