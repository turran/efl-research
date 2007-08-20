#ifndef _CONTEXT_H
#define _CONTEXT_H

/**
 * @file
 * @brief Draw Context
 * @defgroup Draw_Context_Internal Draw Context
 * @ingroup Internal_Group Internal
 * 
 * @todo
 * - Add a fill abstraction into the context, solid color, mul, etc
 * - What about using another surface as the context src pixel, that
 *   will allow to fill an object (rectangle, polygon) with it
 * - replace the Sl_Func with Sp_Func, makes more sense, as it draws
 *   spans no scanlines
 * @{
 *
 */

typedef struct _Emage_Draw_Context_Clip
{
	Emage_Rectangle r;
	char 		use : 1;
} Emage_Draw_Context_Clip;

struct _Emage_Draw_Context
{
	struct {
		char   use : 1;
		DATA32 col;
	} mul;
	struct {
		DATA32 col;
	} col;
	Cutout_Rects cutout;
	struct {
		int color_space;
	} interpolation;
	struct {
		int y, h;
	} sli;
	int            render_op;
	unsigned char  anti_alias : 1;
	Emage_Scaler_Type 	stype;
	Emage_Draw_Context_Clip clip;
	struct {
		Emage_Fill_Type type;
	  	DATA32 color;
		struct {
			Emage_Fill_Surface_Type type;
			Emage_Rectangle srect;
			Emage_Rectangle drect;
			Emage_Surface *s;
		} surface;
	} fill;
};

/** @} */

#endif
