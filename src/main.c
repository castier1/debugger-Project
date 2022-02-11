#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>


int main(int argc, char **argv )
{
	pid_t child = fork();

	if( child == 0 ) {

		printf("CHILD: child done\n");
		exit(1);

	} else {
		wait(NULL);
		printf("PARENT: child done\n");
	}

	return 0;
}