#ifndef _CONTEXT_H
#define _CONTEXT_H

/**
 * @file
 * @brief Draw Context
 * @defgroup Draw_Context_Internal Draw Context
 * @ingroup Internal_Group Internal
 * 
 * @{
 *
 */

/**
 * TODO
 */
typedef struct _Emage_Draw_Context_Clip
{
	Emage_Rectangle r;
	char 		use : 1;
} Emage_Draw_Context_Clip;

/**
 * TODO
 */
struct _Emage_Draw_Context
{
	struct {
		char   use : 1;
		DATA32 col;
	} mul;
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
