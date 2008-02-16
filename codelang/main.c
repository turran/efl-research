#include <unistd.h>

#include <Code.h>

static int loop = 0;

int main(int argc, char **argv)
{
	String *s1, *s2;

	gc_start();

	while (loop < 1)	
	{
		s1 = string_new();
		s2 = string_new();

		STRING_CLASS.set(s1, "foobar");
		STRING_CLASS.clone(s2, s1);

		sleep(6);
		++loop;
	}

	{
		Object *object;

		object = OBJECT(s1);
		OBJECT_CLASS.null(&object);

		object = OBJECT(s2);
		OBJECT_CLASS.null(&object);
	}

	for(;;)
	{
		sleep(2);
	}

	gc_stop();
	return 0;
}
