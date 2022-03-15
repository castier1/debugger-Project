#include <dirent.h>
#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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

int in_syscall = 0;
int counter = 0;

void print_syscall(const pid_t child, int status, int check_status)
{
    if(check_status && WIFEXITED(status)){
        printf("\tChild process stopped.\n");
        return;
    }

    struct user_regs_struct regs;
    if(ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
        perror("\tERROR : print_syscall : PTRACE_GETREGS");

    if(!in_syscall){
        char *str = syscall_name(regs.orig_rax);
        if(str)
            printf("\tSystem Call : %s (rbx:%lld, rcx:%lld, rdx:%lld)\n",
                    str, regs.rbx, regs.rcx, regs.rdx);
        in_syscall = 1;
        counter++;
    }
}

int jump_syscall(const pid_t child, int status, int check_status)
{
    if(check_status && WIFEXITED(status)){
        printf("\tChild process stopped.\n");
        return -1;
    }

    if(ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
        perror("\tERROR : jump_syscall : PTRACE_SYSCALL");

    waitpid(child, &status, 0);
    in_syscall = 0;

    if(check_status)
        printf("\tjump to the next syscall or instruction.\n\t(call 'syscall' for more information)\n");
    return status;
}

void print_all_syscall(const pid_t child, int status)
{
    int check_status = 0;
    siginfo_t sig;

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
    printf("\tChild process stopped.\n");
}

void print_dump(const char *filename, const char *input)
{
    char *str;

    if(strlen(input)) {
        str = malloc(sizeof(*str) * (strlen(input) + 36));
        sprintf(str, "objdump -d %s | sed '/<%s>:/,/^$/!d'", filename, input);
    }
    else {
        str = malloc(sizeof(*str) * (strlen(input) + 12));
        sprintf(str, "objdump -d %s", filename);
    }

    if(system(str) < 0)
        printf("\tERROR: print_dump: system ('%s', '%s')", filename, input);

    free(str);
}

void print_file_descr(const pid_t child)
{
    // Retrieve the directory name
    char dirname[20];
    sprintf(dirname, "/proc/%d/fd/", child);

    DIR* dir = opendir(dirname);
    if(!dir)
        perror("\tERROR : print_open_files : opendir");

    struct dirent * entry = NULL;
    while((entry = readdir(dir)) != NULL)
    {
        //printf("%s of type %s\n", entry->d_name, file_type(entry->d_type));
        if(entry->d_type == DT_LNK)
        {
            char filename[512], linkname[512];
            sprintf(filename, "/proc/%d/fd/%s", child, entry->d_name);

            struct stat stat;
            if((lstat(filename, &stat)) == -1)
                perror("\tERROR : print_open_files : lstat");

            if((readlink(filename, linkname, stat.st_size + 1)) == -1)
                perror("\tERROR : print_open_files : readlink");

            //linkname[49] = '\0';
            printf("\t%s\n", linkname);

        }
    }
    closedir(dir);

}

void print_lib(const pid_t child)
{
    // Retrieve the filename
    char filename[20];
    sprintf(filename, "/proc/%d/maps", child);

    // Open the file
    FILE *file;
    file = fopen(filename, "r");
    if(!file)
        perror("\tERROR : print_lib : unable to open /proc/[pid]/maps");

    char buff[512], libname[100], old_libname[100] = "null";

    // Parse the file, line by line
    while(fgets(buff, 512, file))
    {
        // Parse (and split) the string, and keep only the last part
        char delim[] = " ";
        char *ptr = strtok(buff, delim);
        while(ptr != NULL)
        {
            strcpy(libname, ptr);
            ptr = strtok(NULL, delim);
        }

        // Check if already found and keep only path to libraries (to remove [stack], [heap], etc)
        if(strcmp(libname, old_libname) != 0 && libname[0] == '/')
            printf("\t%s", libname);

        strcpy(old_libname, libname);
    }

    // Close the file
    fclose(file);
}

void print_filetype(const mode_t mode)
{
    char buf[5];

    // SOCKET
    if(S_ISSOCK(mode))
        strcpy(buf, "sock");
    // SYMBOLIC LINK
    else if(S_ISLNK(mode))
        strcpy(buf, "link");
    // REGULAR FILE
    else if(S_ISREG(mode))
        strcpy(buf, "file");
    // BLOCK SPECIAL or CHARACTER SPECIAL
    else if(S_ISBLK(mode) || S_ISCHR(mode))
        strcpy(buf, "devc");
    // REPOSITORY
    else if(S_ISDIR(mode))
        strcpy(buf, "repy");
    // FIFO
    else if(S_ISFIFO(mode))
        strcpy(buf, "fifo");
    // UNDEFINED
    else
        strcpy(buf, "und.");

    printf("\ttype : %s\n", buf);
}

void print_mode(const mode_t mode)
{
    printf("\tmode : %c%c%c%c%c%c%c%c%c\n",
        // S_I[Read/Write/eXecute][USeR/GRouP/OTHer]
        (S_IRUSR & mode) ? 'r' : '-',
        (S_IWUSR & mode) ? 'w' : '-',
        (S_IXUSR & mode) ? 'x' : '-',
        (S_IRGRP & mode) ? 'r' : '-',
        (S_IWGRP & mode) ? 'w' : '-',
        (S_IXGRP & mode) ? 'x' : '-',
        (S_IROTH & mode) ? 'r' : '-',
        (S_IWOTH & mode) ? 'w' : '-',
        (S_IXOTH & mode) ? 'x' : '-');
}

void print_owner(uid_t uid)
{
    struct passwd *pwd;
    pwd = getpwuid(uid);
    printf("\towner : %s\n", pwd->pw_name);
}

void print_metadata(const char *filename)
{
    struct stat stat;
    if(lstat(filename, &stat) == -1)
        perror("\tERROR : print_metadata : lstat");

    // NAME
    printf("\tname : %s\n", filename);
    // TYPE
    print_filetype(stat.st_mode);
    // MODE
    print_mode(stat.st_mode);
    // OWNER
    print_owner(stat.st_uid);
    // SIZE
    printf("\tsize : %ld\n", stat.st_size);
    // TIMES
    printf("\tlast status change : %s", ctime(&stat.st_ctime));
    printf("\tlast file access : %s", ctime(&stat.st_atime));
    printf("\tlast file modification : %s", ctime(&stat.st_mtime));
}

void *open_elf(const char *filename)
{
    void *start = NULL;
    struct stat stat;

    // Open file in Read-only
    int fd = open(filename, O_RDONLY, 660);
    if(fd < 0)
        perror("\tERROR : open_elf : open");

    // Capture the metadata of the file and stock it in stat structure
    if(fstat(fd, &stat) == -1)
        perror("\tERROR : open_elf : fstat");

    // mmap in memory the file and stock the adress of the start of the file
    start = mmap(0, stat.st_size, PROT_READ , MAP_FILE | MAP_SHARED, fd, 0);
    if(start == MAP_FAILED)
    {
        perror("\tERROR : open_elf : mmap");
        abort();
    }

    // Verify that this is an ELF-file (check the firsts bytes)
    if(*(char*)start != 0x7f || *((char*)start+1) != 'E'
       || *((char*)start+2) != 'L' || *((char*)start+3) != 'F')
    {
        perror("\tERROR : open_elf : not an ELF file");
        return NULL;
    }

    // Return a pointer to the start of the mmap file
    return start;
}

void parse_symtab(const char *filename, unsigned char TYPE)
{
    const char *to_ignore[2] = {"", "crtstuff.c"};
    char *strtab;
    int nb_symbols;
    void *start = NULL;

    // Open the elf-file
    start = open_elf(filename);
    if(!start)
        perror("\tERROR : get_source_file : can't retrieve data");

    // Capture (and cast) a pointer on the headers
    Elf64_Ehdr* hdr = (Elf64_Ehdr *) start;
    // Capture (and cast) a pointer on the sections
    Elf64_Shdr *sections = (Elf64_Shdr*)((char*)start + hdr->e_shoff);
    Elf64_Sym *symtab;

    // Parse all the sections
    for (int i = 0; i < hdr->e_shnum; i++)
    {
        // Stock only the datas in the table of symbols
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym*)((char*)start + sections[i].sh_offset);
            nb_symbols = sections[i].sh_size / sections[i].sh_entsize;

            strtab = (char*)((char*)start + sections[sections[i].sh_link].sh_offset);
        }
    }
    switch(TYPE)
    {
        case STT_FILE:
            // Parse all the symbols stocked
            for (int i = 0; i < nb_symbols; ++i)
            {
                // If it's type is "FILE", check it and print it
                if(symtab[i].st_info == STT_FILE)
                {
                    char *tmp = strtab + symtab[i].st_name;
                    if(strcmp(tmp, to_ignore[0]) != 0 && strcmp(tmp, to_ignore[1]) != 0)
                        printf("\tfile source : %s\n", tmp);
                }
            }
            break;

        case STT_FUNC:
            // Parse all the symbols stocked
            for (int i = 0; i < nb_symbols; ++i)
            {
                // If it's type is "FUNC", check it and print it
                if(symtab[i].st_info == STT_FUNC || symtab[i].st_info == 18)
                    printf("\t%s\n", strtab + symtab[i].st_name);
            }
            break;
    }
}

void print_pwd(const char *filename)
{
    char* path = realpath(filename, NULL);
    if(!path)
        perror("\tERROR : print_pwd : realpath");
    else {
        printf("\t%s\n", path);
        free(path);
    }
}

void where_am_i(const char *file, const char *function, const int line)
{
    // To call with : __FILE__, __FUNCTION__, __LINE__
    fprintf(stdout, "\tfile: %s, function : %s, line : %d\n", file, function, line);
}

char *print_si_code(const int si_signo, const int si_code)
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
    // If signo == 0, the signal detected is not an error.
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

void getsignal(const pid_t child)
{
    siginfo_t sig;

    // Fill the siginfo_t structure
    if(ptrace(PTRACE_GETSIGINFO, child, NULL, &sig) == -1)
        perror("\tERROR : getsignal : PTRACE_GETSIGINFO");

    // Print the signal and it's short description
    char *s = print_si_code(sig.si_signo, sig.si_code);
    printf("\t%s %s\n", strsignal(sig.si_signo), s);

    // Print the addresse where the signal was raised
    if(sig.si_signo != SIGTRAP)
        printf("\tadrr = %p\n", &sig.si_addr);

    free(s);
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
        next\t to jump to the next syscall instruction\n");
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
    int run = 1, check_status = 1;
    char input[20], args[20];
    const char *options[17] = {"help", "exit", "run", "signal", "PID",
                               "PPID", "GID", "PGID", "pwd", "file",
                               "meta", "lib", "fd", "func", "dump",
                               "syscall", "next"};

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
            parse_symtab(filename, STT_FILE);
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
            parse_symtab(filename, STT_FUNC);
        // DUMP
        else if(strcmp(input, options[14]) == 0)
        {
            print_dump(filename, args);
            args[0] = '\0';
        }
        // SYSCALL
        else if(strcmp(input, options[15]) == 0)
        {
            if(strcmp(args, "all") == 0) {
                print_all_syscall(child, status);
                args[0] = '\0';
            }
            else
                print_syscall(child, status, check_status);
            printf("\tTotal count of syscall = %d\n", counter);
        }
        // NEXT
        else if(strcmp(input, options[16]) == 0)
            jump_syscall(child, status, check_status);
        else
            printf("\t\"%s\" : unknown command\n", input);
    }
    return 0;
}