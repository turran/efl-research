#ifndef _CODE_MEMBER_H
#define _CODE_MEMBER_H

typedef struct member_s member_t;

typedef enum
{
	MEMBER_ALREADY_DEFINED,
	MEMBER_ILLEGAL_NAME,
	MEMBER_NOERROR
} member_error_t;  

struct member_s
{
	char *name;
	type_t type;
	access_t access;
};

int member_parse(char *name);
void member_access_set(member_t member, access_t access);
void member_type_set(member_t member, type_t type);

#endif
