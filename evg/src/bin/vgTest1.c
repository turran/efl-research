#include "openvg.h"

const VGubyte star_segs[] = 
{
		VG_MOVE_TO_ABS,
		VG_LINE_TO_ABS,
		VG_LINE_TO_ABS,
		VG_LINE_TO_ABS,
		VG_LINE_TO_ABS,
		VG_CLOSE_PATH
};

const VGfloat star_coords[] =
{
		170, 42.85,
		90, 354.28,
		332.85, 92.85,
		37.14, 107.14,
		334.28, 324.28
};


int main(void)
{
	EVGContext c;
	VGImage i;
	VGPath p;
	
	/* create a new context */
	c = evgContextNew();
	evgContextSet(c);
	
	/* create a few paths */
	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 6, 5, VG_PATH_CAPABILITY_ALL);
	i = vgCreateImage(VG_lARGB_8888_PRE, 400, 400,
			VG_IMAGE_QUALITY_NONANTIALIASED | VG_IMAGE_QUALITY_FASTER | VG_IMAGE_QUALITY_BETTER);
	evgSetTargetSurface(i);
	vgDrawPath(p, VG_FILL_PATH);
	/* delete the context */
	
	evgContextFree(c);
	return 0;
}
