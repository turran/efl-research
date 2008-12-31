/*
 * etk2_types.h
 *
 *  Created on: Dec 15, 2008
 *      Author: hisham
 */

#ifndef _ETK2_TYPES_H_
#define _ETK2_TYPES_H_

typedef struct _Type Type;
typedef struct _Type_Property Type_Property;
typedef struct _Type_Property_Value Type_Property_Value;

typedef void (*Type_Constructor)(Type *, void *);
typedef void (*Type_Destructor)(void *);

typedef void (*Type_Property_Process)(void *);

typedef struct _Object Object;

typedef void (*Object_Property_Value_Set)(Object *object, char *prop_name, Type_Property_Value *value);
typedef Type_Property_Value *(*Object_Property_Value_Get)(Object *object, char *prop_name);

typedef struct _Widget Widget;
typedef struct _Button Button;
typedef struct _Label Label;

#endif
