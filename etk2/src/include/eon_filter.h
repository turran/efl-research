/*
 * eon_image.h
 *
 *  Created on: 26-feb-2009
 *      Author: jl
 */
#ifndef EON_FILTER_H_
#define EON_FILTER_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
Ekeko_Property_Id EON_FILTER_EFFECT_X;
Ekeko_Property_Id EON_FILTER_EFFECT_Y;
Ekeko_Property_Id EON_FILTER_EFFECT_W;
Ekeko_Property_Id EON_FILTER_EFFECT_H;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Filter_Private Eon_Filter_Private;
struct _Eon_Filter
{
	Ekeko_Object parent;
	Eon_Filter_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_filter_type_get(void);
EAPI Eon_Filter * eon_filter_new(Eon_Canvas *c);

#endif
