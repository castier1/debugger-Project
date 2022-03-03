#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../header/lib.h"

int main(int argc, char **argv )
{
	int status;
	pid_t child = fork();

	if(argc != 2)
	{
		printf("USAGE: ./main <prog_to_analyse>\n");
		return 1;
	}

	if( child == 0 ) {
		// If in Child process
		if(run_prog(argv[1]) == -1)
			printf("ERROR: main: run_prog\n");

		printf("CHILD: child done\n");

	} else {
		//If in Parent process
		waitpid(child, &status, 0);
		if(start_UI(child) == -1)
			printf("ERROR: main: start_UI\n");

		printf("PARENT: child done\n");
	}

	return 0;
}