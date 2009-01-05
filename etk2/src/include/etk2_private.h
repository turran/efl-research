/*
 * etk_private.h
 *
 *  Created on: 14-Dec-2008
 *      Author: Hisham Mardam-Bey <hisham.mardambey@gmail.com>
 */

#ifndef _ETK2_PRIVATE_H
#define _ETK2_PRIVATE_H

#define RETURN_IF(expr) if ((expr)) return
#define RETURN_NULL_IF(expr) if ((expr)) return NULL

#define OFFSET(type, mem) ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

Type * object_private_type_get(Object *object);
void object_construct(Type *type, void *instance);
void type_construct(Type *t, void *instance);
void * type_instance_private_get_internal(Type *final, Type *t, void *instance);

#endif
