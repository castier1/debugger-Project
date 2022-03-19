#pragma once

// Print the name of all the file descriptor opened
void print_file_descr(const pid_t child);

// Print all the loaded dynamic librairies
void print_lib(const pid_t child);

// Print the stack list of a pid
void print_stack(pid_t child);