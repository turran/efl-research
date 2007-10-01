#ifndef _READER_H
#define _READER_H

/**
 *
 */
struct _Enginy_Component_Reader
{
	Enginy_Point 		*points;
	char 			*cmds;
	
	int 			pos;

	Enginy_Component 	*from; 	/**< Component the reader reads from */
	Enginy_Component 	*to; 	/**< In case the reader is used as a pipe */
	
};

void enginy_reader_notify(Enginy_Component_Reader *r);
void enginy_reader_reference_update(Enginy_Component_Reader *r);

#endif
