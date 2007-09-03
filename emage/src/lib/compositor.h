#ifndef _COMPOSITOR_H
#define _COMPOSITOR_H

/** 
 * @file
 * @brief Compositors
 * @defgroup Compositors_Internal Compositor
 * @ingroup Internal_Group Internal
 *
 * @todo
 * - instead of 32 b color, why not some functions for each data format
 *   to get the current color
 * - use another render operation based on different parameters:
 *   if render op = blend and use color span with color 0xffxxxxxxxx then 
 *   the render op should be a copy not a blend, etc
 * - Merge pixel_color and pixel into one, the second is just an special 
 *   case where the drawing context uses the mul color
 * - Remove pixel_color in compositor.c, it's a special case for pixel 
 *   with mul.use, the compositor still need to provide a function for it
 * - The above also matches the mask case? if so the dc needs a mask
 * - One way to improve the speed on the scalers is to change how the scanlines
 *   functions work, instead of passing only the offset we should pass also
 *   the step for each surface, for not having to add the offset every time
 * - Port all compositors from evas common engine
 * 
 * @{
 *
 */

void emage_compositor_init(void);
void emage_compositor_blend_init(void);
void emage_compositor_blend_rel_init(void);
void emage_compositor_copy_init(void);


typedef void (*Emage_Sl_Func) (Emage_Surface *src, int soffset, DATA8 *mask, int moffset, DATA32 col, Emage_Surface *dst, int doffset, int len);
typedef void (*Emage_Pt_Func) (Emage_Surface *src, int soffset, DATA8 mask, DATA32 col, Emage_Surface *dst, int doffset);

#define SL_NAME(op, cs, cpu, name) op ## _ ## cs ## _ ## cpu ## _sl_ ## name
#define PT_NAME(op, cs, cpu, name) op ## _ ## cs ## _ ## cpu ## _pt_ ## name

#define SL_FUNC(op, cs, cpu, name) 					\
void SL_NAME(op, cs, cpu, name)(Emage_Surface *src, int soffset, 	\
	DATA8 *mask, int moffset, DATA32 col, Emage_Surface *dst, 	\
	int doffset, int len)

#define PT_FUNC(op, cs, cpu, name) 					\
void PT_NAME(op, cs, cpu, name)(Emage_Surface *src, int soffset, 	\
	DATA8 mask, DATA32 col, Emage_Surface *dst, int doffset)

#define COMPOSITOR_OVERRIDE_ALL(c, op, cs, cpu) 			\
	c->sl_pixel 		= SL_NAME(op, cs, cpu, pixel); 		\
	c->sl_color 		= SL_NAME(op, cs, cpu, color); 		\
	c->sl_pixel_color 	= SL_NAME(op, cs, cpu, pixel_color);	\
	c->sl_mask_color 	= SL_NAME(op, cs, cpu, mask_color);	\
	c->sl_pixel_mask 	= SL_NAME(op, cs, cpu, pixel_mask);	\
	c->pt_pixel 		= PT_NAME(op, cs, cpu, pixel); 		\
	c->pt_color 		= PT_NAME(op, cs, cpu, color); 		\
	c->pt_pixel_color 	= PT_NAME(op, cs, cpu, pixel_color); 	\
	c->pt_mask_color 	= PT_NAME(op, cs, cpu, mask_color);	\
	c->sl_pixel_mask 	= PT_NAME(op, cs, cpu, pixel_mask);

#define COMPOSITOR_PROTO_ALL(op, cs, cpu)	 			\
SL_FUNC(op, cs, cpu, pixel); 						\
SL_FUNC(op, cs, cpu, color); 						\
SL_FUNC(op, cs, cpu, pixel_color);					\
SL_FUNC(op, cs, cpu, mask_color);					\
SL_FUNC(op, cs, cpu, pixel_mask);					\
PT_FUNC(op, cs, cpu, pixel); 						\
PT_FUNC(op, cs, cpu, color); 						\
PT_FUNC(op, cs, cpu, pixel_color); 					\
PT_FUNC(op, cs, cpu, mask_color);					\
PT_FUNC(op, cs, cpu, pixel_mask);

/* our dummy functions in case the compositor doesnt implement them */
void dummy_sl(Emage_Surface *src, int soffset, DATA8 *mask, int moffset, DATA32 col, Emage_Surface *dst, int doffset, int len);
void dummy_pt(Emage_Surface *src, int soffset, DATA8 mask, DATA32 col, Emage_Surface *dst, int doffset);

/* TODO document why it was choosen only one level of _get instead of two as 
 * evas_common had
 * FIXME
 * The above functions should be of type Sl_Func and Pt_Func only the compositor
 * wrapper should have different types for each function
 */
typedef struct _Emage_Compositor
{
	/* Scanlines */
	Emage_Sl_Func sl_pixel;
	Emage_Sl_Func sl_color;
	Emage_Sl_Func sl_pixel_color;
	Emage_Sl_Func sl_mask_color;
	Emage_Sl_Func sl_pixel_mask;
	/* Points */
	Emage_Pt_Func pt_pixel;
	Emage_Pt_Func pt_color;
	Emage_Pt_Func pt_pixel_color;
	Emage_Pt_Func pt_mask_color;
	Emage_Pt_Func pt_pixel_mask;
} Emage_Compositor;

#define _EVAS_RENDER_FILL        -1

extern Emage_Compositor Emage_Compositors[EMAGE_DATA_FORMATS][EMAGE_RENDER_OPS];

/** @} */

#endif

