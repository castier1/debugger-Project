#include <unistd.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>


#include "../header/lib.h"

int run_prog(const char *filename)
{
	if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
	{
        perror("ERROR: run_prog: ptrace");
        return -1;
    }

    if (execl(filename, filename, 0, NULL) == -1)
	{
        perror("ERROR: run_prog: ptrace");
        return -1;
    }
	return 0;
}


void helpMsg()
{
	printf("%s\n%s\n%s\n",
		"help\t to show this message",
		"exit\t to quit this interface",
		"");
}

int start_UI()
{
	int run = 1;
	char input[20];
	const char *options[3] = {"help", "exit"};

	while(run)
	{
		printf("analyse >>> ");
        scanf("%s", input);

        if(strcmp(input, options[0]) == 0)
            helpMsg();
        else if(strcmp(input, options[1]) == 0)
            run = 0;
	}

	return 0;
}