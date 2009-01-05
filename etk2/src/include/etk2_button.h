#ifndef _BUTTON_H
#define _BUTTON_H

typedef struct _Button Button;
typedef struct _Button_Private Button_Private;

struct _Button
{
	Widget widget;
	Button_Private *private;
};

Button     *button_new(void);
void        button_label_set(Button *button, const char *label);
const char *button_label_get(Button *button);
Type       *button_type_get(void);
#endif
