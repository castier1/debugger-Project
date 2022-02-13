#include <stdlib.h>
#include <stdio.h>

void sigsegv_function(int a)
{
	// Create a Segmentation fault
	int * p = NULL;
	*p = a;
}

int main(int argc, char const *argv[])
{
	int var = 12;

	sigsegv_function(var);

	return 0;
}