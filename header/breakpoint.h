#pragma once

typedef struct breakpoint{
    long func_addr;         // address of the breakpoint
    char func_name[20];     // name of the function to stop
    int func_data;          // data stored at the address originally
} func_bt;

//
void add_bt(pid_t child, long int addr);

//
void remove_bt(pid_t child, long int addr);

//
void list_bt(func_bt* array_bt, int count);

