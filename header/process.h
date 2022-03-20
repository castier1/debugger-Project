#pragma once

// Print the stack list of a pid
void print_stack(const pid_t child);

// Print the name of all the file descriptor opened
void print_file_descr(const pid_t child);

// Print all the loaded dynamic librairies
void print_lib(const pid_t child);

// Get the starting adress of the stack of a pid
long get_start_stack_addr(const pid_t child);