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
 *
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
	/*struct Emage_Draw_Context_clip {
		int    x, y, w, h;
		char   use : 1;
	} clip;*/
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
};

/** @} */

#endif
