/*
 * eon_sqpattern.h
 *
 *  Created on: 22-jul-2009
 *      Author: jl
 */

#ifndef EON_SQPATTERN_H_
#define EON_SQPATTERN_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define EON_SQPATTERN_SW_CHANGED "swChanged"
#define EON_SQPATTERN_SH_CHANGED "shChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_SQPATTERN_SW;
extern Ekeko_Property_Id EON_SQPATTERN_SH;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Sqpattern_Private Eon_Sqpattern_Private;
struct _Eon_Sqpattern
{
	Eon_Paint parent;
	Eon_Sqpattern_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_sqpattern_type_get(void);
EAPI Eon_Sqpattern * eon_sqpattern_new(void);

#endif /* EON_SQPATTERN_H_ */
