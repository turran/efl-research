/*
 * etk2_types.h
 *
 *  Created on: Dec 15, 2008
 *      Author: hisham
 */

#ifndef _ETK2_TYPES_H_
#define _ETK2_TYPES_H_

typedef struct _Type Type;
typedef struct _Object Object;
typedef struct _Container Container;

typedef void (*Type_Constructor)(void *);
typedef void (*Type_Destructor)(void *);

typedef void (*Type_Property_Process)(void *);

typedef void (*Object_Property_Value_Set)(Object *object, char *prop_name, Value *value);
typedef Value *(*Object_Property_Value_Get)(Object *object, char *prop_name);

#endif
