#ifndef ENESIM_RENDERER_H_
#define ENESIM_RENDERER_H_

/**
 * @defgroup Enesim_Renderer_Group Renderer
 * @{
 */
typedef struct _Enesim_Renderer Enesim_Renderer; /**< Renderer Handler */

EAPI void enesim_renderer_rop_set(Enesim_Renderer *r, int rop);
EAPI void enesim_renderer_delete(Enesim_Renderer *r);
EAPI Eina_Bool enesim_renderer_draw(Enesim_Renderer *r, int type, void *sl, Enesim_Surface *dst);
/**
 * @defgroup Enesim_Renderer_Fill_Color_Group Fill Color
 * @{
 */
EAPI Enesim_Renderer * enesim_fill_color_new(void);
EAPI void enesim_fill_color_color_set(Enesim_Renderer *r, DATA32 color);
/** 
 * @} 
 * @defgroup Enesim_Renderer_Fill_Surface_Group Fill Surface
 * @{
 */
enum
{
	ENESIM_SURFACE_REPEAT_NONE  = 0, /**< Don't repeat on any axis */
	ENESIM_SURFACE_REPEAT_X     = (1 << 0), /**< Repeat on X axis */
	ENESIM_SURFACE_REPEAT_Y     = (1 << 1), /**< Repeat on Y axis */
};
EAPI Enesim_Renderer * enesim_fill_surface_new(void);
EAPI void enesim_fill_surface_surface_set(Enesim_Renderer *r, Enesim_Surface *s);
EAPI void enesim_fill_surface_mode_set(Enesim_Renderer *r, int mode);
EAPI void enesim_fill_surface_dst_area_set(Enesim_Renderer *r, int x, int y, int w, int h);
EAPI void enesim_fill_surface_src_area_set(Enesim_Renderer *r, int x, int y, int w, int h);
/** 
 * @}
 * @}
 */

#endif /*ENESIM_RENDERER_H_*/
