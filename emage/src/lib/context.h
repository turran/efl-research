#ifndef _CONTEXT_H
#define _CONTEXT_H

/**
 * @file
 * @brief Draw Context
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

#endif
