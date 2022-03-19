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

void print_stack(pid_t child)
{
    // Retrieve the cmd
    char cmd[30];
    sprintf(cmd, "sudo cat /proc/%d/stack", child);

    // Execute with sudo rights
    system(cmd);
}

void print_file_descr(const pid_t child)
{
    // Retrieve the directory name
    char dirname[20];
    sprintf(dirname, "/proc/%d/fd/", child);

    DIR* dir = opendir(dirname);
    if(!dir)
        perror("\tERROR : print_open_files : opendir");

    // Parse all the entries in the directory
    struct dirent * entry = NULL;
    while((entry = readdir(dir)) != NULL)
    {
        // If entry is a symbolic link
        if(entry->d_type == DT_LNK)
        {
            // Retrieve it's name
            char filename[512], linkname[512];
            sprintf(filename, "/proc/%d/fd/%s", child, entry->d_name);

            // Retrieve it's size
            struct stat stat;
            if((lstat(filename, &stat)) == -1)
                perror("\tERROR : print_open_files : lstat");

            // Retrieve the file source
            if((readlink(filename, linkname, stat.st_size + 1)) == -1)
                perror("\tERROR : print_open_files : readlink");

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