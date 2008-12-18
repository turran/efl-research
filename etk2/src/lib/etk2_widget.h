#ifndef _WIDGET_H
#define _WIDGET_H

#include "etk2_types.h"
#include "etk2_type.h"
#include "etk2_object.h"

typedef struct _Widget_Private Widget_Private;

struct _Widget
{
	Object object;
	Widget_Private *private;
};

Widget *widget_new(void);
Type   *widget_type_get(void);
void    widget_geom_get(Widget *widget, int *x, int *y, int *w, int *h);
void    widget_geom_set(Widget *widget, int x, int y, int w, int h);

#endif
