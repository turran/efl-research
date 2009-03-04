/*
 * ekeko_main.h
 *
 *  Created on: 30-ene-2009
 *      Author: jl
 */
#ifndef EKEKO_MAIN_H_
#define EKEKO_MAIN_H_

#define EKEKO_CAST_CHECK

#ifdef EKEKO_CAST_CHECK
#define EKEKO_CAST(o, e_type, c_type) ((c_type *)(ekeko_object_cast((Ekeko_Object *)o, (e_type))))
#else
#define EKEKO_CAST(o, e_type, c_type) ((c_type *)(o))
#endif

EAPI int ekeko_init(void);
EAPI int ekeko_shutdown(void);

#endif /* EKEKO_MAIN_H_ */
