#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <Code.h>

#define PASSED(class, test) printf("["#class"]  ["#test"]...  [PASSED]\n");

void string_new_test()
{
	String *s1 = NULL;

	s1 = string_new();

	assert((s1 != NULL));
	assert((OBJECT(s1)->ref_count == 1));
	assert((s1->class == &String_Class));
	PASSED(String, new);
}

void string_set_test()
{
	String *s1, *s2;

	s1 = string_new();

	STRING_CLASS.set(s1, "foobar");

	assert((s1->value != NULL));
	assert((strcmp(s1->value, "foobar") == 0));
	PASSED(String, set);
}

void string_clone_test()
{
	String *s1, *s2;

	s1 = string_new();
	s2 = string_new();
	
	STRING_CLASS.set(s1, "foobar");
	STRING_CLASS.clone(s2, s1);
	
	assert((s2->value != NULL));
	assert((strcmp(s2->value, "foobar") == 0));
	PASSED(String, clone);
}

void string_assign_test()
{
	String *s1, *s2;

	s1 = string_new();
	s2 = string_new();

	STRING_CLASS.set(s1, "foobar");
	STRING_CLASS.assign(s2, s1);;

	assert((s2->value != NULL));
	assert((strcmp(s2->value, "foobar") == 0));
	assert((s2->value == s1->value));
	PASSED(String, assign);
}

void string_delete_test()
{
	String *s1;

	s1 = string_new();
	STRING_CLASS.set(s1, "foobar");
	STRING_CLASS.delete(&s1);

	assert((s1 == NULL));
	PASSED(String, delete);
}

void string_dtor_test()
{
	String *s1;

	s1 = string_new();
	STRING_CLASS.set(s1, "foobar");
	STRING_CLASS.dtor(s1);

	assert((s1->value == NULL));
	PASSED(String, dtor);
}

int main(int argc, char **argv)
{
	string_new_test();
	string_set_test();
	string_clone_test();
	string_assign_test();
	string_delete_test();
	string_dtor_test();

	return 0;
}
