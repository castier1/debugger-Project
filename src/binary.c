#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void print_dump(const char *filename, const char *input)
{
    char *str;

    // If there is a function name as an option, dump just this function
    if(strlen(input)) {
        str = malloc(sizeof(*str) * (strlen(filename) + strlen(input) + 36) + 1);
        sprintf(str, "objdump -d %s | sed '/<%s>:/,/^$/!d'", filename, input);
    }
    // If no options, dump all the binary
    else {
        str = malloc(sizeof(*str) * (strlen(filename) + 12) + 1);
        sprintf(str, "objdump -d %s", filename);
    }

    if(system(str) < 0)
        printf("\tERROR: print_dump: system ('%s', '%s')", filename, input);

    free(str);
}

void print_function_infos(const char *filename, const size_t addr)
{
    char *str;
    // 23 = strlen("addr2line -pe %s -f %lx")
    // 16 = strlen(addr)
    str = malloc(sizeof(*str) * (strlen(filename) + 23 + 16) + 1);
    if(!str)
        perror("ERROR : print_function_infos: malloc");

    sprintf(str, "addr2line -pe %s -f %lx", filename, addr);

    if(system(str) < 0)
        printf("\tERROR: print_function_infos: system ('%s', '%lx')", filename, addr);

    free(str);
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

long int parse_symtab(const char *filename, const unsigned char TYPE, const char *func_name, int ret_addr)
{
    const char *to_ignore[2] = {"", "crtstuff.c"};
    char *strtab;
    int nb_symbols;
    void *start = NULL;

    // Open the elf-file
    start = open_elf(filename);
    if(!start)
        return perror("\tERROR : get_source_file : can't retrieve data"), -1;

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
            // Stock the names
            symtab = (Elf64_Sym*)((char*)start + sections[i].sh_offset);
            // Stock the number of symbols found
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
                if((symtab[i].st_info == STT_FUNC || symtab[i].st_info == 18)
                    && symtab[i].st_value)
                {
                    char *tmp = strtab + symtab[i].st_name;
                    if(!func_name)
                        printf("\t(0x%lx)\t%s\n", symtab[i].st_value, strtab + symtab[i].st_name);
                    else if(strcmp(func_name, tmp) == 0){
                        if(!ret_addr){
                            print_function_infos(filename, symtab[i].st_value);
                            break;
                        }
                        else
                            return symtab[i].st_value;
                    }
                }
            }
            break;
        case STT_OBJECT:
            // Parse all the symbols stocked
            for (int i = 0; i < nb_symbols; ++i)
            {
                // If it's type is "FUNC", check it and print it
                if(symtab[i].st_info == STT_OBJECT && symtab[i].st_size > 0)
                {
                    char *tmp = strtab + symtab[i].st_name;
                    printf("\t%s (size = %ld)\n", tmp, symtab[i].st_size);
                }
            }
            break;
    }
    return 0;
}
