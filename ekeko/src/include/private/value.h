/*
 * value.h
 *
 *  Created on: 30-ene-2009
 *      Author: jl
 */

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

#ifndef VALUE_H_
#define VALUE_H_

void ekeko_value_init(void);
void ekeko_value_shutdown(void);
void ekeko_value_pointer_double_set(Value *value, Value_Type type, void *ptr,
		void *prev, char *changed);
void ekeko_value_pointer_set(Value *v, Value_Type vtype, void *ptr);

#endif /* VALUE_H_ */
