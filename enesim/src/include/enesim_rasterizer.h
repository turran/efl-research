#ifndef ENESIM_RASTERIZER_H_
#define ENESIM_RASTERIZER_H_


/**
 * @defgroup Enesim_Rasterizer_Group Rasterizer
 * @{
 */

enum
{
	ENESIM_OK = 1,
	ENESIM_ERROR_SCANLINE_NOT_SUPPORTED,
};

typedef enum
{
	ENESIM_RASTERIZER_FILL_RULE_EVENODD,
	ENESIM_RASTERIZER_FILL_RULE_NONEZERO,
	ENESIM_RASTERIZER_FILL_RULES
} Enesim_Rasterizer_Fill_Rule;

typedef struct _Enesim_Rasterizer Enesim_Rasterizer; /**< Rasterizer Handler */

EAPI void enesim_rasterizer_vertex_add(Enesim_Rasterizer *r, float x, float y);
EAPI int enesim_rasterizer_generate(Enesim_Rasterizer *r, Enesim_Scanline_Callback cb, void *data);
EAPI void enesim_rasterizer_delete(Enesim_Rasterizer *r);
EAPI void enesim_rasterizer_fill_rule_set(Enesim_Rasterizer *r, Enesim_Rasterizer_Fill_Rule rule);

EAPI Enesim_Rasterizer * enesim_rasterizer_cpsc_new(Enesim_Rectangle boundaries);

typedef enum
{
	ENESIM_RASTERIZER_KIIA_COUNT_8,
	ENESIM_RASTERIZER_KIIA_COUNT_16,
	ENESIM_RASTERIZER_KIIA_COUNT_32,
	ENESIM_RASTERIZER_KIIA_COUNTS
} Enesim_Rasterizer_Kiia_Count;

EAPI Enesim_Rasterizer * enesim_rasterizer_kiia_new(Enesim_Rasterizer_Kiia_Count count,
		Enesim_Rectangle boundaries);

/** @} */ //End of Enesim_Rasterizer_Group



#endif /*ENESIM_RASTERIZER_H_*/
