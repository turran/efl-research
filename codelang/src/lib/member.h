#ifndef _CODE_MEMBER_H
#define _CODE_MEMBER_H

typedef enum
{
	MEMBER_ALREADY_DEFINED,
	MEMBER_ILLEGAL_NAME,
	MEMBER_NOERROR
} member_error_t;  

int parse_member(char *name);

#endif
