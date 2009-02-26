#ifndef ETK2_COORD_H_
#define ETK2_COORD_H_

static inline void etk_coord_set(Etk_Coord *c, int value, Etk_Coord_Type type)
{
	c->value = value;
	c->type = type;
}

#endif /* ETK2_COORD_H_ */
