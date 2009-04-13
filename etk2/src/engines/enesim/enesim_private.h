#ifndef ENESIM_PRIVATE_H_
#define ENESIM_PRIVATE_H_

typedef struct _Context
{
	uint32_t color;
	Enesim_Rop rop;
	struct {
		Eina_Rectangle rect;
		Eina_Bool used;
	} clip;

} Context;

#endif /* ENESIM_PRIVATE_H_ */
