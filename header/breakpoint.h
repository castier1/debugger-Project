#pragma once

typedef struct breakpoint{
	int avail;
    void *addr;         // address of the breakpoint
    char name[20];     // name of the function to stop
    int data;          // data stored at the address originally
    struct breakpoint *next;
} func_bp;

// Add a breakpoint
void add_bt(pid_t child, func_bp* bp);

// Create a breakpoint
int create_bp(const char* filename, const pid_t child, func_bp* bp, const char * func_name, void *addr);

//
int delete_bp(func_bp* list_bp, int pos);

// Remove a breakpoint
int remove_bp(pid_t child, func_bp* bp, int pos);

// List all the breakpoints created
void list_all_bp(func_bp* array_bp, int count);

//
void free_list_bp(func_bp* list_bp, int count);