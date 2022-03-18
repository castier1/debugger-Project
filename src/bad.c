#include <stdlib.h>
#include <stdio.h>

static const int variable_globale = 100;

void sigsegv_function(int a)
{
	// Create a Segmentation fault
	int * variable_func = NULL;
	*variable_func = a;
}

int main(int argc, char const *argv[])
{
	int variable_main = 12;

	sigsegv_function(variable_main);

	return 0;
}
