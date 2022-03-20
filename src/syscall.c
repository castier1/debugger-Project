#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/user.h>
#include <sys/wait.h>

char *syscall_name(long long int id)
{
    switch(id){
        case SYS_exit: return "SYS_exit"; break;
        case SYS_fork: return "SYS_fork"; break;
        case SYS_read: return "SYS_read"; break;
        case SYS_open: return "SYS_open"; break;
        case SYS_wait4: return "SYS_wait4"; break;
        case SYS_chdir: return "SYS_chdir"; break;
        case SYS_chown: return "SYS_chown"; break;
        case SYS_lseek: return "SYS_lseek"; break;
        case SYS_getpid: return "SYS_getpid"; break;
        case SYS_mount: return "SYS_mount"; break;
        case SYS_setuid: return "SYS_setuid"; break;
        case SYS_getuid: return "SYS_getuid"; break;
        case SYS_geteuid: return "SYS_geteuid"; break;
        case SYS_ptrace: return "SYS_ptrace"; break;
        case SYS_access: return "SYS_access"; break;
        case SYS_sync: return "SYS_sync"; break;
        case SYS_kill: return "SYS_kill"; break;
        case SYS_stat: return "SYS_stat"; break;
        case SYS_getppid: return "SYS_getppid"; break;
        case SYS_lstat: return "SYS_lstat"; break;
        case SYS_dup: return "SYS_dup"; break;
        case SYS_pipe: return "SYS_pipe"; break;
        case SYS_getegid: return "SYS_getegid"; break;
        case SYS_getgid: return "SYS_getgid"; break;
        case SYS_acct: return "SYS_acct"; break;
        case SYS_ioctl: return "SYS_ioctl"; break;
        case SYS_symlink: return "SYS_symlink"; break;
        case SYS_readlink: return "SYS_readlink"; break;
        case SYS_execve: return "SYS_execve"; break;
        case SYS_chroot: return "SYS_chroot"; break;
        case SYS_fstat: return "SYS_fstat"; break;
        case SYS_msync: return "SYS_msync"; break;
        case SYS_brk: return "SYS_brk"; break;
        case SYS_mmap: return "SYS_mmap"; break;
        case SYS_munmap: return "SYS_munmap"; break;
        case SYS_mprotect: return "SYS_mprotect"; break;
        case SYS_madvise: return "SYS_madvise"; break;
        case SYS_mincore: return "SYS_mincore"; break;
        case SYS_getgroups: return "SYS_getgroups"; break;
        case SYS_setgroups: return "SYS_setgroups"; break;
        case SYS_getpgrp: return "SYS_getpgrp"; break;
        case SYS_setpgid: return "SYS_setpgid"; break;
        case SYS_setitimer: return "SYS_setitimer"; break;
        case SYS_swapon: return "SYS_swapon"; break;
        case SYS_getitimer: return "SYS_getitimer"; break;
        case SYS_sethostname: return "SYS_sethostname"; break;
        case SYS_dup2: return "SYS_dup2"; break;
        case SYS_fcntl: return "SYS_fcntl"; break;
        case SYS_select: return "SYS_select"; break;
        case SYS_fsync: return "SYS_fsync"; break;
        case SYS_setpriority: return "SYS_setpriority"; break;
        case SYS_socket: return "SYS_socket"; break;
        case SYS_connect: return "SYS_connect"; break;
        case SYS_getpriority: return "SYS_getpriority"; break;
        case SYS_rt_sigreturn: return "SYS_sigreturn"; break;
        case SYS_bind: return "SYS_bind"; break;
        case SYS_setsockopt: return "SYS_setsockopt"; break;
        case SYS_listen: return "SYS_listen"; break;
        case SYS_rt_sigsuspend: return "SYS_rt_sigsuspend"; break;
        case SYS_sigaltstack: return "SYS_sigaltstack"; break;
        case SYS_gettimeofday: return "SYS_gettimeofday"; break;
        case SYS_getrusage: return "SYS_getrusage"; break;
        case SYS_getsockopt: return "SYS_getsockopt"; break;
        case SYS_readv: return "SYS_readv"; break;
        case SYS_writev: return "SYS_writev"; break;
        case SYS_settimeofday: return "SYS_settimeofday"; break;
        case SYS_fchown: return "SYS_fchown"; break;
        case SYS_fchmod: return "SYS_fchmod"; break;
        case SYS_setreuid: return "SYS_setreuid"; break;
        case SYS_setregid: return "SYS_setregid"; break;
        case SYS_rename: return "SYS_rename"; break;
        case SYS_truncate: return "SYS_truncate"; break;
        case SYS_ftruncate: return "SYS_ftruncate"; break;
        case SYS_flock: return "SYS_flock"; break;
        case SYS_sendto: return "SYS_sendto"; break;
        case SYS_shutdown: return "SYS_shutdown"; break;
        case SYS_socketpair: return "SYS_socketpair"; break;
        case SYS_mkdir: return "SYS_mkdir"; break;
        case SYS_rmdir: return "SYS_rmdir"; break;
        case SYS_utimes: return "SYS_utimes"; break;
        case SYS_adjtimex: return "SYS_adjtimex"; break;
        case SYS_getrlimit: return "SYS_getrlimit"; break;
        case SYS_setrlimit: return "SYS_setrlimit"; break;
        case SYS_setsid: return "SYS_setsid"; break;
        case SYS_quotactl: return "SYS_quotactl"; break;
        case SYS_statfs: return "SYS_statfs"; break;
        case SYS_fstatfs: return "SYS_fstatfs"; break;
        case SYS_shmat: return "SYS_shmat"; break;
        case SYS_setgid: return "SYS_setgid"; break;
        default: return NULL; break;
    }
}

static int in_syscall = 0;
static int counter = 0;

void print_syscall(const pid_t child, int status, int check_status)
{
    // Check if the process is already stopped
    if(check_status && WIFEXITED(status)){
        printf("\tChild process stopped.\n");
        return;
    }

    // Catch and stock the register data
    struct user_regs_struct regs;
    if(ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
        perror("\tERROR : print_syscall : PTRACE_GETREGS");

    // If just before a syscall, print it's name and description
    if(!in_syscall){
        char *str = syscall_name(regs.orig_rax);
        if(str){
            printf("\t%s (%ld, %ld, %ld, %ld, %ld, %ld)\n",
                    str, (long)regs.rdi, (long)regs.rsi, (long)regs.rdx,
                         (long)regs.r10, (long)regs.r8,  (long)regs.r9);
        }
        in_syscall = 1;
        counter++;
    }
}

int jump_syscall(const pid_t child, int status, int check_status)
{
    // Check if the process is already stopped
    if(check_status && (WIFEXITED(status) || WIFSTOPPED(status))){
        printf("\tChild process stopped.\n");
        return -1;
    }

    // Jump to the next syscall
    if(ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
        perror("\tERROR : jump_syscall : PTRACE_SYSCALL");

    // Wait the end of the execution
    waitpid(child, &status, 0);
    in_syscall = 0;

    if(check_status)
        printf("\tjump to the next syscall or instruction.\n\t(call 'syscall' for more information)\n");
    return status;
}

void print_all_syscall(const pid_t child, int status)
{
    // Disable some comments
    int check_status = 0;
    siginfo_t sig;

    // While the program is running
    while(!WIFEXITED(status))
    {
        // Find and print syscall
        print_syscall(child, status, check_status);
        //Jump to the next syscall
        status = jump_syscall(child, status, check_status);

        // Check signal
        if(ptrace(PTRACE_GETSIGINFO, child, NULL, &sig) == -1)
            perror("\tERROR : print_all_syscall : PTRACE_GETSIGINFO");
        // If signo != 0, detection of an error and stop the process
        if(sig.si_signo != SIGTRAP){
            printf("\t%s %d %d\n", strsignal(sig.si_signo), sig.si_signo, sig.si_code);
            break;
        }
    }
    printf("\tTotal count of syscall = %d\n", counter);
    printf("\tChild process stopped.\n");
}