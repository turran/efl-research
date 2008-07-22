#include "Eshm.h"

#define S_4M (4 * 1024 * 1024)

int main(int argc, char **argv)
{
	Eshm_Segment *s;
	
	eshm_init();
	s = eshm_segment_new("mysegment", S_4M);
	//ecore_main_loop_begin();
	eshm_shutdown();
	
	return 0;
}
