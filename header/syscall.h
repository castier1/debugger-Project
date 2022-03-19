#pragma once

// Get the syscall name
char *syscall_name(long long int id);

// Print the syscall and and print it's description
void print_syscall(const pid_t child, int status, int check_status);

// Jump to the next syscall
int jump_syscall(const pid_t child, int status, int check_status);

// Run the program and print all it's execution
void print_all_syscall(const pid_t child, int status);