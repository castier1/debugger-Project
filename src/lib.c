#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "../header/lib.h"


void exec_prog(const char *filename)
{
    if (execl(filename, filename, 0, NULL) == -1)
        perror("\tERROR: exec_prog: ptrace");
}


int run_prog(const char *filename)
{
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
        return perror("\tERROR: run_prog: ptrace"), -1;

    exec_prog(filename);
    
    return 0;
}

void where_am_i(const char *file, const char *function, const int line)
{
    // To call with : __FILE__, __FUNCTION__, __LINE__
    fprintf(stdout, "\tfile: %s, function : %s, line : %d\n", file, function, line);
}


void print_pwd(char *filename)
{
    char* path = realpath(filename, NULL);
    if(!path)
        printf("\tcannot find %s\n", filename);
    else{
        printf("\t%s\n", path);
        free(path);
    }
}

char *print_si_code(int si_signo, int si_code)
{
    char *s = calloc(76, sizeof(*s));

    int in = 0;

    switch(si_signo){
        case SIGILL:
            switch(si_code){
                case ILL_ILLOPC:
                    sprintf(s, ": Illegal opcode.(%d)", si_code);
                    break;
                case ILL_ILLOPN:
                    sprintf(s, ": Illegal operand.(%d)", si_code);
                    break;
                case ILL_ILLADR:
                    sprintf(s, ": Illegal addressing mode.(%d)", si_code);
                    break;
                case ILL_ILLTRP:
                    sprintf(s, ": Illegal trap.(%d)", si_code);
                    break;
                case ILL_PRVOPC:
                    sprintf(s, ": Privileged opcode.(%d)", si_code);
                    break;
                case ILL_PRVREG:
                    sprintf(s, ": Privileged register.(%d)", si_code);
                    break;
                case ILL_COPROC:
                    sprintf(s, ": Coprocessor error.(%d)", si_code);
                    break;
                case ILL_BADSTK:
                    sprintf(s, ": Internal stack error.(%d)", si_code);
                    break;
            }
            break;
        case SIGFPE:
            switch(si_code){
                case FPE_INTDIV:
                    sprintf(s, ": Integer divide by zero.(%d)", si_code);
                    break;
                case FPE_INTOVF:
                    sprintf(s, ": Integer overflow.(%d)", si_code);
                    break;
                case FPE_FLTDIV:
                    sprintf(s, ": Floating-point divide by zero.(%d)", si_code);
                    break;
                case FPE_FLTOVF:
                    sprintf(s, ": Floating-point overflow.(%d)", si_code);
                    break;
                case FPE_FLTUND:
                    sprintf(s, ": Floating-point underflow.(%d)", si_code);
                    break;
                case FPE_FLTRES:
                    sprintf(s, ": Floating-point inexact result.(%d)", si_code);
                    break;
                case FPE_FLTINV:
                    sprintf(s, ": Invalid floating-point operation.(%d)", si_code);
                    break;
                case FPE_FLTSUB:
                    sprintf(s, ": Subscript out of range.(%d)", si_code);
                    break;
            }
            break;
        case SIGSEGV:
            switch(si_code){
                case SEGV_MAPERR:
                    sprintf(s, ": Address not mapped to object.(%d)", si_code);
                    break;
                case SEGV_ACCERR:
                    sprintf(s, ": Invalid permissions for mapped object.(%d)", si_code);
                    break;
            }
            break;
        case SIGBUS:
            switch(si_code){
                case BUS_ADRALN:
                    sprintf(s, ": Invalid address alignment.(%d)", si_code);
                    break;
                case BUS_ADRERR:
                    sprintf(s, ": Nonexistent physical address.(%d)", si_code);
                    break;
                case BUS_OBJERR:
                    sprintf(s, ": Object-specific hardware error.(%d)", si_code);
                    break;
            }
            break;
        case SIGTRAP:
            sprintf(s, ": Process trace trap.(%d)", si_code);
            break;
        case SIGCHLD:
            switch(si_code){
                case CLD_EXITED:
                    sprintf(s, ": Child has exited.(%d)", si_code);
                    break;
                case CLD_KILLED:
                    sprintf(s, ": Child has terminated abnormally and did not create a core file.(%d)", si_code);
                    break;
                case CLD_DUMPED:
                    sprintf(s, ": Child has terminated abnormally and created a core file.(%d)", si_code);
                    break;
                case CLD_TRAPPED:
                    sprintf(s, ": Traced child has trapped.(%d)", si_code);
                    break;
                case CLD_STOPPED:
                    sprintf(s, ": Child has stopped.(%d)", si_code);
                    break;
                case CLD_CONTINUED:
                    sprintf(s, ": Stopped child has continued.(%d)", si_code);
                    break;
            }
            break;
        case SIGPOLL:
            switch(si_code){
                case POLL_IN:
                    sprintf(s, ": Data input available.(%d)", si_code);
                    break;
                case POLL_OUT:
                    sprintf(s, ": Output buffers available.(%d)", si_code);
                    break;
                case POLL_MSG:
                    sprintf(s, ": Input message available.(%d)", si_code);
                    break;
                case POLL_ERR:
                    sprintf(s, ": I/O error.(%d)", si_code);
                    break;
                case POLL_PRI:
                    sprintf(s, ": High priority input available.(%d)", si_code);
                    break;
                case POLL_HUP:
                    sprintf(s, ": Device disconnected.(%d)", si_code);
                    break;
            }
            break;
        default:
            in = 1;
            break;
    }

    if (in){
        switch(si_code){
            case SI_USER:
                sprintf(s, ": Signal sent by kill().(%d)", si_code);
                break;
            case SI_QUEUE:
                sprintf(s, ": Signal sent by the sigqueue().(%d)", si_code);
                break;
            case SI_TIMER:
                sprintf(s, ": Signal generated by expiration of a timer set by timer_settime().(%d)", si_code);
                break;
            case SI_ASYNCIO:
                sprintf(s, ": Signal generated by completion of an asynchronous I/O request.(%d)", si_code);
                break;
            case SI_MESGQ:
                sprintf(s, ": Signal generated by arrival of a message on an empty message queue.(%d)", si_code);
                break;
            default:
                s = NULL;
                break;
        }
    }

    return s;
}

void getsignal(pid_t child)
{
    siginfo_t sig;

    if(ptrace(PTRACE_GETSIGINFO, child, NULL, &sig) == -1)
        perror("\tERROR : getsignal : PTRACE_GETSIGINFO");

    char *s = print_si_code(sig.si_signo, sig.si_code);
    printf("\t%s %s\n", strsignal(sig.si_signo), s);
    printf("\tadrr = %p\n", &sig.si_addr);

    free(s);
}

void helpMsg()
{
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        "\thelp\t to show this message",
        "\texit\t to quit this interface",
        "\trun\t to run the program",
        "\tsignal\t to list the last signal received",
        "\tPID\t to print the PID",
        "\tPPID\t to print the PPID",
        "\tGID\t to print the GID",
        "\tpwd\t to print the absolute path of the program to analyse");
}

void kill_child_process(pid_t child)
{
    if(ptrace(PTRACE_KILL, child, 0, 0) == -1)
        perror("\tERROR : resume : PTRACE_CONT");
    else
        printf("\tProcess %d killed\n", child);
    sleep(1);

}

void resume(pid_t child)
{
    if(ptrace(PTRACE_CONT, child, 0, 0) == -1)
        perror("\tERROR : resume : PTRACE_CONT");
    sleep(1);

}

int start_UI(pid_t child, gid_t gid, char *filename)
{
    int run = 1;
    char input[20];
    const char *options[8] = {"help", "exit", "run", "signal", "PID", "PPID", "GID", "pwd"};

    while(run)
    {
        printf("analyse >>> ");
        scanf("%s", input);

        if(strcmp(input, options[0]) == 0)
            helpMsg();
        else if(strcmp(input, options[1]) == 0){
            run = 0;
            kill_child_process(child);
        }
        else if(strcmp(input, options[2]) == 0)
            resume(child);
        else if(strcmp(input, options[3]) == 0)
            getsignal(child);
        else if(strcmp(input, options[4]) == 0)
            printf("\t %d\n", child);
        else if(strcmp(input, options[5]) == 0)
            printf("\t %d\n", getpid());
        else if(strcmp(input, options[6]) == 0)
            printf("\t %d\n", gid);
        else if(strcmp(input, options[7]) == 0)
            print_pwd(filename);
        else
            printf("\t\"%s\" : unknown command\n", input);
    }
    return 0;
}