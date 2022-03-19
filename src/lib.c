//#include <dirent.h>
#include <elf.h>
#include <fcntl.h>
//#include <limits.h>
//#include <pwd.h>
//#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/stat.h>
//#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
//#include <time.h>
#include <unistd.h>

#include "../header/metadata.h"
#include "../header/signal.h"
#include "../header/binary.h"
#include "../header/process.h"
#include "../header/syscall.h"
#include "../header/lib.h"

void exec_prog(char * const *argv)
{
    // argv = path_to_file, argument1, argument2, ..., NULL
    if (execv(argv[0], argv) == -1)
        perror("\tERROR: exec_prog: execv");
}

int run_prog(char * const* argv)
{
    // Enable the ptrace system and stop (pause) the execution of this process
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
        return perror("\tERROR: run_prog: ptrace"), -1;

    exec_prog(argv);
    return 0;
}

void where_am_i(const char *file, const char *function, const int line)
{
    // To call with : __FILE__, __FUNCTION__, __LINE__
    fprintf(stdout, "\tfile: %s, function : %s, line : %d\n", file, function, line);
}

void helpMsg()
{
    printf("\thelp\t to show this message\n\
        exit\t to quit this interface\n\
        run\t to run the program\n\
        signal\t to print the last signal received\n\
        PID\t to print the PID\n\
        PPID\t to print the Parent PID\n\
        GID\t to print the GID\n\
        PGID\t to print the Parent GID\n\
        pwd\t to print the absolute path of the program to analyse\n\
        file\t to print the name of the source code file\n\
        meta\t to print all the metadata of the file to analyse (type, mode, owner, size, time)\n\
        lib\t to print the list of all the dynamic librairies loaded\n\
        fd\t to print all the file descriptor opened\n\
        func\t to retrieve all the the function of the file to analyse\n\
        dump [<func>]\t to dump all the program or just a given function (need objdump)\n\
        syscall [all]\t to check if there is a syscall at the time\n\
        \t\t (if 'all' option, then run and print all the syscall of the program)\n\
        next\t to jump to the next syscall instruction\n\
        locate <func>\t to print the location (file and line) of a given function (need addr2line)\n\
        stack\t to print the state of the program-to-analyse's stack (need sudo rights)\n\
        gvar\t to print all the global data in the program\n");
}

void kill_child_process(const pid_t child)
{
    // Kill the child process to stop it
    if(ptrace(PTRACE_KILL, child, 0, 0) == -1)
        perror("\tERROR : kill_child_process : PTRACE_KILL");
    else
        printf("\tProcess %d killed\n", child);

    // Wait 1 sec
    sleep(1);

}

void resume(const pid_t child)
{
    // Resume the execution of the child process
    if(ptrace(PTRACE_CONT, child, 0, 0) == -1)
        perror("\tERROR : resume : PTRACE_CONT");

    // Wait 1 sec
    sleep(1);

}

int start_UI(const pid_t child, int stat, const char *filename)
{
    int status = stat;
    int run = 1, check_status = 1, count = 0;
    char input[20], args[20];
    const char *options[20] = {"help", "exit", "run", "signal", "PID",
                               "PPID", "GID", "PGID", "pwd", "file",
                               "meta", "lib", "fd", "func", "dump",
                               "syscall", "next", "locate", "stack",
                               "gvar"};

    while(run)
    {
        printf("analyse >>> ");

        char buffer[50];
        fgets(buffer, 50, stdin);
        sscanf(buffer, "%s %s", input, args);

        // HELP
        if(strcmp(input, options[0]) == 0)
            helpMsg();
        // EXIT
        else if(strcmp(input, options[1]) == 0)
        {
            run = 0;
            kill_child_process(child);
        }
        // RUN
        else if(strcmp(input, options[2]) == 0)
            resume(child);
        // SIGNAL
        else if(strcmp(input, options[3]) == 0)
            getsignal(child);
        // PID
        else if(strcmp(input, options[4]) == 0)
            printf("\t %d\n", child);
        // PPID
        else if(strcmp(input, options[5]) == 0)
            printf("\t %d\n", getpid());
        // GID
        else if(strcmp(input, options[6]) == 0)
            printf("\t %d\n", getgid());
        // PGID
        else if(strcmp(input, options[7]) == 0)
            printf("\t %d\n", getgid());
        // PWD
        else if(strcmp(input, options[8]) == 0)
            print_pwd(filename);
        // FILE
        else if(strcmp(input, options[9]) == 0)
            parse_symtab(filename, STT_FILE, NULL);
        // META
        else if(strcmp(input, options[10]) == 0)
            print_metadata(filename);
        // LIB
        else if(strcmp(input, options[11]) == 0)
            print_lib(child);
        // FD
        else if(strcmp(input, options[12]) == 0)
            print_file_descr(child);
        // FUNC
        else if(strcmp(input, options[13]) == 0)
            parse_symtab(filename, STT_FUNC, NULL);
        // DUMP
        else if(strcmp(input, options[14]) == 0)
        {
            print_dump(filename, args);
            args[0] = '\0';
            sleep(1);
        }
        // SYSCALL
        else if(strcmp(input, options[15]) == 0)
        {
            // If there is the 'all' option
            if(strcmp(args, "all") == 0) {
                print_all_syscall(child, status);
                args[0] = '\0';
            }
            // If no option
            else
                print_syscall(child, status, check_status);
        }
        // NEXT
        else if(strcmp(input, options[16]) == 0)
            jump_syscall(child, status, check_status);
        // LOCATE
        else if(strcmp(input, options[17]) == 0)
        {
            // If no argument
            if(strcmp(args, "") == 0)
                printf("\tUSAGE : locate <function>\n\
            Use the 'func' command to list all the possible function.\n");
            // If there is an argument
            else {
                parse_symtab(filename, STT_FUNC, args);
                args[0] = '\0';
            }
        }
        // STACK
        else if(strcmp(input, options[18]) == 0)
            print_stack(child);
        // GLOBAL VARIABLE
        else if(strcmp(input, options[19]) == 0)
            parse_symtab(filename, STT_OBJECT, NULL);
        else
            printf("\t\"%s\" : unknown command\n", input);
    }
    return 0;
}