#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../header/lib.h"

int main(int argc, char **argv )
{
	int status;
	pid_t child = 0;
	gid_t gid = 0;

	if(argc != 2)
		return printf("USAGE: ./main <prog_to_analyse>\n"), 1;


    child = fork();
    if (child < 0)
        return printf("ERROR : main : fork failed"), 1;
	else if(child == 0 )
	{
		// If in Child process
		if(run_prog(argv[1]) == -1)
			printf("ERROR: main: run_prog\n");

		gid = getgid();

		printf("CHILD: child done\n");
		exit(0);
	}
	else
	{
		//If in Parent process
		waitpid(child, &status, 0);
		if(start_UI(child, gid, argv[1]) == -1)
			printf("ERROR: main: start_UI\n");

		printf("PARENT: child done\n");
	}

	return 0;
}