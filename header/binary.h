#pragma once

// Print the result of all or a part of the objump command
void print_dump(const char *filename, const char *input);

// Print the result of the addr2line command
void print_function_infos(const char *filename, const size_t addr);

// Check and open an elf file
void *open_elf(const char *filename);

// Parse an elf file and retrieve data: source file name or functions name
void *parse_symtab(const char *filename, const unsigned char TYPE, const char *func_name, int ret_addr);