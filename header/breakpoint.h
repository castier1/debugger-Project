#pragma once

typedef struct breakpoint{
    long int addr;                // address of the breakpoint
    char name[30];             // name of the function to stop
    int data;                  // data stored at the address originally
    struct breakpoint *next;   // next breakpoint created (or NULL)
} func_bp;

// Parse the list of breakpoints
int check_exist(func_bp *list_bp, const char *func_name);

// Add a breakpoint in a process
void add_bt(pid_t child, func_bp *bp);

// Create a breakpoint structure
int create_bp(const char *filename, const pid_t child, func_bp **bp, const char *func_name);

// Delete a breakpoint structure
int delete_bp(func_bp **list_bp, int pos);

// Remove a breakpoint from a process
int remove_bp(pid_t child, func_bp **bp, const char *func_name);

// List all the breakpoints created
void list_all_bp(func_bp *array_bp, int count);

// Deallocate all memory
void free_list_bp(func_bp **list_bp, int count);