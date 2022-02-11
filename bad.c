#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("bad.c -- debut\n");
	int * p = NULL;
	*p = 12;
	printf("bad.c -- fin\n");
	return 0;
}