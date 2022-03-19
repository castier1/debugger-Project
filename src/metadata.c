#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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

void print_pwd(const char *filename)
{
    // Retrieve the absolute path
    char* path = realpath(filename, NULL);

    // Check errors and print it
    if(!path)
        perror("\tERROR : print_pwd : realpath");
    else {
        printf("\t%s\n", path);
        free(path);
    }
}