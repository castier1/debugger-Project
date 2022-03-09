#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../header/lib.h"

int main(int argc, char **argv)
{
	int status;
	pid_t child = 0;
	gid_t gid = 0;

	if(argc == 1)
		return printf("USAGE : ./analyzer [--args] <prog_to_analyse> [arg1 arg2 ...]\n"), 1;


    child = fork();
    // If fork failed
    if (child < 0)
        return printf("ERROR : main : fork failed"), 1;
    // If in Child process
	else if(child == 0)
	{
		// Run the program given in parameters
		if(run_prog(argv[1]) == -1)
			return printf("ERROR : main : run_prog\n"), 1;

		// Save the GroupID
		gid = getgid();
		exit(0);
	}
	//If in Parent process
	else {
		// Wait the child process stop
		if(waitpid(child, &status, 0) == -1)
			return printf("ERROR : main : waitpid\n"), 1;
		// Start the communication with user
		if(start_UI(child, gid, argv[1]) == -1)
			printf("ERROR : main : start_UI\n");
	}

	return 0;
}