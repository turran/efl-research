#include "Codelang.h"

member_t last_member;

int member_parse(char *name)
{
	return MEMBER_NOERROR;
}

void member_access_set(member_t member, access_t access)
{
	member.access = access;
}
