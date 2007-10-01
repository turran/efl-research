#ifndef _READER_H
#define _READER_H

/**
 *
 */
struct _Equis_Component_Reader
{
	Equis_Point 		*points;
	char 			*cmds;
	
	int 			pos;

	Equis_Component 	*from; 	/**< Component the reader reads from */
	Equis_Component 	*to; 	/**< In case the reader is used as a pipe */
	
};

void equis_reader_notify(Equis_Component_Reader *r);
void equis_reader_reference_update(Equis_Component_Reader *r);

#endif
