#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>


#include "../header/lib.h"

// Run the given programm to analyse
int run_prog(char *filename)
{	
	return -1;
}

// Start the analyse (with user's interface)
int start_UI()
{
	return -1;
}

int main(int argc, char **argv )
{
	int status;
	pid_t child = fork();

	if( child == 0 ) {
		// If in Child process
		if(run_prog(NULL) == -1)
			printf("ERROR: main: run_prog\n");
		printf("CHILD: child done\n");

	} else {
		//If in Parent process
		waitpid(child, &status, 0);
		if(start_UI() == -1)
			printf("ERROR: main: start_UI\n");;
		printf("PARENT: child done\n");
	}

	return 0;
}