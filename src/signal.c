#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

char *print_si_code(const int si_signo, const int si_code)
{
    switch(si_signo){
        case SIGILL:
            switch(si_code){
                case ILL_ILLOPC:
                    return ": Illegal opcode";
                    break;
                case ILL_ILLOPN:
                    return ": Illegal operand.";
                    break;
                case ILL_ILLADR:
                    return ": Illegal addressing mode.";
                    break;
                case ILL_ILLTRP:
                    return ": Illegal trap.";
                    break;
                case ILL_PRVOPC:
                    return ": Privileged opcode.";
                    break;
                case ILL_PRVREG:
                    return ": Privileged register.";
                    break;
                case ILL_COPROC:
                    return ": Coprocessor error.";
                    break;
                case ILL_BADSTK:
                    return ": Internal stack error.";
                    break;
            }
            break;
        case SIGFPE:
            switch(si_code){
                case FPE_INTDIV:
                    return ": Integer divide by zero.";
                    break;
                case FPE_INTOVF:
                    return ": Integer overflow.";
                    break;
                case FPE_FLTDIV:
                    return ": Floating-point divide by zero.";
                    break;
                case FPE_FLTOVF:
                    return ": Floating-point overflow.";
                    break;
                case FPE_FLTUND:
                    return ": Floating-point underflow.";
                    break;
                case FPE_FLTRES:
                    return ": Floating-point inexact result.";
                    break;
                case FPE_FLTINV:
                    return ": Invalid floating-point operation.";
                    break;
                case FPE_FLTSUB:
                    return ": Subscript out of range.";
                    break;
            }
            break;
        case SIGSEGV:
            switch(si_code){
                case SEGV_MAPERR:
                    return ": Address not mapped to object.";
                    break;
                case SEGV_ACCERR:
                    return ": Invalid permissions for mapped object.";
                    break;
            }
            break;
        case SIGBUS:
            switch(si_code){
                case BUS_ADRALN:
                    return ": Invalid address alignment.";
                    break;
                case BUS_ADRERR:
                    return ": Nonexistent physical address.";
                    break;
                case BUS_OBJERR:
                    return ": Object-specific hardware error.";
                    break;
            }
            break;
        case SIGTRAP:
            return ": Process trace trap.";
            break;
        case SIGCHLD:
            switch(si_code){
                case CLD_EXITED:
                    return ": Child has exited.";
                    break;
                case CLD_KILLED:
                    return ": Child has terminated abnormally and did not create a core file.";
                    break;
                case CLD_DUMPED:
                    return ": Child has terminated abnormally and created a core file.";
                    break;
                case CLD_TRAPPED:
                    return ": Traced child has trapped.";
                    break;
                case CLD_STOPPED:
                    return ": Child has stopped.";
                    break;
                case CLD_CONTINUED:
                    return ": Stopped child has continued.";
                    break;
            }
            break;
        case SIGPOLL:
            switch(si_code){
                case POLL_IN:
                    return ": Data input available.";
                    break;
                case POLL_OUT:
                    return ": Output buffers available.";
                    break;
                case POLL_MSG:
                    return ": Input message available.";
                    break;
                case POLL_ERR:
                    return ": I/O error.";
                    break;
                case POLL_PRI:
                    return ": High priority input available.";
                    break;
                case POLL_HUP:
                    return ": Device disconnected.";
                    break;
            }
            break;
        default:
            break;
    }
    // If signo == 0, the signal detected is not an error.
    switch(si_code){
        case SI_USER:
            return ": Signal sent by kill().";
            break;
        case SI_QUEUE:
            return ": Signal sent by the sigqueue().";
            break;
        case SI_TIMER:
            return ": Signal generated by expiration of a timer set by timer_settime().";
            break;
        case SI_ASYNCIO:
            return ": Signal generated by completion of an asynchronous I/O request.";
            break;
        case SI_MESGQ:
            return ": Signal generated by arrival of a message on an empty message queue.";
            break;
        default:
            return NULL;
            break;
    }
}

void getsignal(const pid_t child, const int status)
{
    // Check if the process is already stopped
    if(WIFEXITED(status)){
        printf("\tProcess %d stopped.\n", child);
        return;
    }

    siginfo_t sig;

    // Fill the siginfo_t structure
    if(ptrace(PTRACE_GETSIGINFO, child, NULL, &sig) == -1)
        perror("\tERROR : getsignal : PTRACE_GETSIGINFO");

    // Print the signal and it's short description
    printf("\t%s %s\n",
           strsignal(sig.si_signo),
           print_si_code(sig.si_signo, sig.si_code));

    // Print the addresse where the signal was raised
    if(sig.si_signo != SIGTRAP)
        printf("\tadrr = %p\n", &sig.si_addr);
}