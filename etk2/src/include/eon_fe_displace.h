/*
 * eon_fe_displace.h
 *
 *  Created on: 07-may-2009
 *      Author: jl
 */

#ifndef EON_FE_DISPLACE_H_
#define EON_FE_DISPLACE_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_FE_DISPLACE_SRC;
extern Ekeko_Property_Id EON_FE_DISPLACE_MAP;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Fe_Displace_Private Eon_Fe_Displace_Private;
struct _Eon_Fe_Displace
{
	Eon_Filter parent;
	Eon_Fe_Displace_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_fe_displace_type_get(void);
EAPI Eon_Fe_Displace * eon_fe_displace_new(Eon_Canvas *c);

#endif /* EON_FE_DISPLACE_H_ */
