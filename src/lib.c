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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "../header/lib.h"


void exec_prog(char * const *argv)
{
    if (execv(argv[0], argv) == -1)
        perror("\tERROR: exec_prog: ptrace");
}

int run_prog(char * const* argv)
{
    // Enable the ptrace system and stop (pause) the execution of this process
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
        return perror("\tERROR: run_prog: ptrace"), -1;

    exec_prog(argv);
    return 0;
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
    lstat(filename, &stat);

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
    fstat(fd, &stat);

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

void get_source_file(const char *filename)
{
    const char *to_ignore[2] = {"", "crtstuff.c"};
    char *strtab;
    int nb_symbols;
    void *start = NULL;

    char *source = NULL;
    source = malloc(15 * sizeof(*source));

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
    // Parse all the symbols stocked
    for (int i = 0; i < nb_symbols; ++i)
    {
        // If it's type is "FILE", check it and print it
        if(symtab[i].st_info == STT_FILE)
        {
            char *tmp = strtab + symtab[i].st_name;
            if(strcmp(tmp, to_ignore[0]) != 0 && strcmp(tmp, to_ignore[1]) != 0)
                strcpy(source, tmp);
        }
    }

    printf("\tfile source : %s\n", source);
    free(source);
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
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        "\thelp\t to show this message",
        "\texit\t to quit this interface",
        "\trun\t to run the program",
        "\tsignal\t to print the last signal received",
        "\tPID\t to print the PID",
        "\tPPID\t to print the Parent PID",
        "\tGID\t to print the GID",
        "\tPGID\t to print the Parent GID",
        "\tpwd\t to print the absolute path of the program to analyse",
        "\tfile\t to print the name of the source code file",
        "\tmeta\t to print all the metadata of the file to analyse\
           \n\t\t  (file type, mode, owner, file size, times)");
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

int start_UI(const pid_t child, const gid_t gid, const char *filename)
{
    int run = 1;
    char input[20];
    const char *options[11] = {"help", "exit", "run", "signal", "PID",
                               "PPID", "GID", "PGID", "pwd", "file",
                               "meta"};

    while(run)
    {
        printf("analyse >>> ");
        scanf("%s", input);

        // HELP
        if(strcmp(input, options[0]) == 0)
            helpMsg();
        // EXIT
        else if(strcmp(input, options[1]) == 0){
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
            printf("\t %d\n", gid);
        // PGID
        else if(strcmp(input, options[7]) == 0)
            printf("\t %d\n", getgid());
        // PWD
        else if(strcmp(input, options[8]) == 0)
            print_pwd(filename);
        // FILE
        else if(strcmp(input, options[9]) == 0)
            get_source_file(filename);
        // META
        else if(strcmp(input, options[10]) == 0)
            print_metadata(filename);
        else
            printf("\t\"%s\" : unknown command\n", input);
    }
    return 0;
}