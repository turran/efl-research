#include "openvg.h"

int main(void)
{
	EVGContext c;
	VGPath p;
	
	/* create a new context */
	c = evgContextNew();
	evgContextSet(c);
	
	/* create a few paths */
	//p = vgPathCreate(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F);
	//vgDrawPath(p, VG_FILL_PATH);
	/* delete the context */
	evgContextFree(c);
	return 0;
}
