#include <stdio.h>

void first_function()
{
	printf("WELCOME TO GOOD !!\n");
}

void second_function(int a)
{
	int i;
	for(i = 0; i < a; ++i)
		printf(" --> i = %d\n", i);
}

int main(int argc, char const *argv[])
{
	int var = 5;

	first_function();
	//second_function(var);

	return 0;
}
