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
void widget_delete(Widget *w);
Type   *widget_type_get(void);
void    widget_geom_get(Widget *widget, int *x, int *y, int *w, int *h);
void    widget_geom_set(Widget *widget, int x, int y, int w, int h);
char *widget_theme_get(Widget *widget);
void widget_theme_set(Widget *widget, char *theme);
#endif
