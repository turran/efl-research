#ifndef _CODE_STRING_H
#define _CODE_STRING_H

#define STRING(ptr) ((String*)(ptr))
#define STRING_CLASS String_Class 

typedef struct _String String;
typedef struct _String_T String_T;

extern String_T String_Class;

struct _String_T
{
	void (*assign)(String *self, String *str);
	void (*ctor)(String *self);
	void (*clone)(String *self, String *str);
	void (*delete)(String **self);
	void (*dtor)(String *self);
	int (*length)(String *self);
	void (*set)(String *self, char *str);
	char *(*to_string)(String *self);
};

struct _String
{
	Object parent;
	String_T *class;

	char *value;
	int _length;
};

String *string_new(void);

#endif
