#pragma once

typedef struct breakpoint{
    long int addr;                // address of the breakpoint
    char name[30];             // name of the function to stop
    long data;                  // data stored at the address originally
    struct breakpoint *next;   // next breakpoint created (or NULL)
} func_bp;

// Parse the list of breakpoints, and return the position if exist
int check_exist(func_bp *list_bp, const char *func_name);

// Add a breakpoint in a process
void add_bt(const pid_t child, func_bp *bp);

// Create a breakpoint structure
int create_bp(const pid_t child, const int status, func_bp **list_bp, const char *func_name, const char *filename);

// Delete a breakpoint structure
int delete_bp(func_bp **list_bp, const int pos);

// Remove a breakpoint from a process
int remove_bp(const pid_t child, const int status, func_bp **list_bp, const char *func_name, int pos);

// List all the breakpoints created
void list_all_bp(func_bp *list_bp, const int count);

// Deallocate all memory
void free_list_bp(const pid_t child, const int status, func_bp **list_bp, const int count);