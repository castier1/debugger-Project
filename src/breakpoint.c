#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>

#include "../header/binary.h"
#include "../header/breakpoint.h"

void add_bt(const pid_t child, func_bp* bp)
{
	// Save the original data
	bp->data = ptrace(PTRACE_PEEKTEXT, child, bp->addr, 0);

	// Add the breakpoint
    ptrace(PTRACE_POKETEXT, child, bp->addr, (bp->data & 0xFFFFFF00) | 0xCC);

    printf("\tBreakpoint added at the %s function, at the %p address.\n", bp->name, bp->addr);
}

int create_bp(const char* filename, const pid_t child, func_bp* list_bp, const char * func_name, void *addr)
{
	// If no address but the name of the function
	if(!addr && func_name){
		addr = parse_symtab(filename, STT_FUNC, func_name, 1);
	}
	// If still no adress
	if(!addr){
		printf("\tCannot find right address for '%s',\
               try to give directly the address.\n", func_name);
		return -1;
	}

	// If first breakpoint
	if(!list_bp) {
		list_bp = malloc(sizeof(*list_bp));

		strcpy(list_bp->name, func_name);
	    list_bp->addr = addr;
	    list_bp->avail = 1;
	    list_bp->next = NULL;

	    // Add the breakpoint in the process
        add_bt(child, list_bp);
	}
	else {
		// Go to the last bp
		func_bp *tmp = list_bp;
		while(tmp->next)
			tmp = tmp->next;

		// Create and stock breakpoint's data
		func_bp *new_bp = malloc(sizeof(*new_bp));

	    strcpy(new_bp->name, func_name);
	    new_bp->addr = addr;
	    new_bp->avail = 1;
	    new_bp->next = NULL;
	    tmp->next = new_bp;

	    // Add the breakpoint in the process
        add_bt(child, new_bp);
	}
    return 0;
}

int delete_bp(func_bp* list_bp, int pos)
{
	if(!list_bp)
		return -1;

	func_bp* tmp = list_bp;
	func_bp *to_delete;

	for (int i = 0; i < pos; ++i)
		tmp = tmp->next;

	to_delete = tmp->next;
	tmp->next = tmp->next->next;

	free(to_delete);
}

int remove_bp(const pid_t child, func_bp* list_bp, int pos)
{
	if(!list_bp)
		return perror("\tNo breakpoints."), -1;

	unsigned int data = 0;
	func_bp* tmp = list_bp;

	// Go to the breakpoint to remove
	for (int i = 1; i < pos; ++i)
		tmp = tmp->next;
	tmp = tmp->next;

	// Get the actual data
	ptrace(PTRACE_PEEKTEXT, child, tmp->addr, 0);

	// Restore the data
    ptrace(PTRACE_POKETEXT, child, tmp->addr, (data & 0xFFFFFF00) | (tmp->data & 0xFF));

    // Delete the breakpoint from the list
    if(delete_bp(list_bp, pos) == -1)
        return -1;

    printf("\tBreakpoint removed at the %s function, at the %p address.\n", tmp->name, tmp->addr);

    return 0;
}

void list_all_bp(func_bp* list_bp, int count)
{
	printf("\t%d breakpoint(s).\n", count);

	func_bp* tmp = list_bp;
	int i = 0;

	while(list_bp && tmp){
		printf("\t%d: %p, %s\n", i, tmp->addr, tmp->name);
		i++;
		tmp = tmp->next;
	}
}

void free_list_bp(func_bp* list_bp, int count)
{
	for (int i = count - 1; i >= 0; ++i)
		delete_bp(list_bp, i);
}