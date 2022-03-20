#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../header/lib.h"

int main(int argc, char *argv[])
{
	int status;
	pid_t child = 0;

	if(argc < 2)
		return printf("USAGE : ./analyzer <prog_to_analyse> [arg1 arg2 ...]\n"), 1;

	argv++;
	argc--;

    child = fork();
    // If fork failed
    if (child < 0)
        return printf("ERROR : main : fork failed"), 1;
    // If in Child process
	else if(child == 0)
	{
		// Run the program given in parameters
		if(run_prog(argv) == -1)
			return printf("ERROR : main : run_prog\n"), 1;

		exit(0);
	}
	//If in Parent process
	else {
		// Wait the child process stop
		if(waitpid(child, &status, 0) == -1)
			return printf("ERROR : main : waitpid\n"), 1;

		// Help the process to distinguish between traps 
		if(ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD) == -1)
			return perror("ERROR : main : PTRACE_SETOPTIONS/PTRACE_O_TRACESYSGOOD"), 1;

		// Start the communication with user
		if(start_UI(child, status, argv[0]) == -1)
			return printf("ERROR : main : start_UI\n"), 1;
	}

	return 0;
}