#ifndef _CODE_METHOD_H
#define _CODE_METHOD_H

typedef struct _Method_T Method_T;

struct _Method_T
{
	char *name;
	int arg_count;
	Argument_T **args;
	Type_T *ret_type;
	Method_Code_T *code;
};



#endif
