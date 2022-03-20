#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>

#include "../header/binary.h"
#include "../header/breakpoint.h"

int check_exist(func_bp *list_bp, const char *func_name)
{
	if(!list_bp)
		return -1;

	int i = 0;
	func_bp *tmp = list_bp;

	while(tmp)
	{
		if(strcmp(tmp->name, func_name) == 0)
			return i;
		tmp = tmp->next;
		i++;
	}
	return 1;
}

void add_bt(const pid_t child, func_bp *bp)
{
	// Save the original data
	bp->data = ptrace(PTRACE_PEEKTEXT, child, (void *)bp->addr, 0);

	// Add the breakpoint
    ptrace(PTRACE_POKETEXT, child, (void *)bp->addr, (bp->data & 0xFFFFFF00) | 0xCC);

    printf("\tBreakpoint added at the %s function, at the 0x%lx address.\n", bp->name, bp->addr);
}

int create_bp(const char *filename, const pid_t child, func_bp **list_bp, const char *func_name)
{
    if(check_exist(*list_bp, func_name) != -1){
		printf("\tAlready exist a breakpoint for '%s'.\n", func_name);
		return -1;
	}

	long int addr = 0;
	// If no address but the name of the function
	if(func_name){
		addr = parse_symtab(filename, STT_FUNC, func_name, 1);
	}
	// If still no adress
	if(!addr){
		printf("\tCannot find right address for '%s',\
        try to give directly the address.\n", func_name);
		return -1;
	}

	// Go to the last bp
	func_bp *tmp = *list_bp;
	while(tmp && tmp->next)
		tmp = tmp->next;

	// Create and stock breakpoint's data
	func_bp *new_bp = malloc(sizeof(*new_bp));

    strcpy(new_bp->name, func_name);
    new_bp->addr = addr;
    new_bp->next = NULL;

    // Add the breakpoint in the process
    add_bt(child, new_bp);

    if(tmp)
	    tmp->next = new_bp;
	else
	    *list_bp = new_bp;

    return 0;
}

int delete_bp(func_bp **list_bp, const int pos)
{
	if(!*list_bp)
		return -1;

	func_bp* tmp = *list_bp;
	func_bp *old_tmp;

	if (pos == 0)
		*list_bp = (*list_bp)->next;
	else
	{
		for (int i = 1; i < pos-1; ++i)
			tmp = tmp->next;

		old_tmp = tmp;
		tmp = tmp->next;
		if(!tmp->next)
		     old_tmp->next = NULL;
		else
		    old_tmp->next = tmp->next;
	}

	free(tmp);
}

int remove_bp(const pid_t child, func_bp **list_bp, const char *func_name)
{
	if(!*list_bp)
		return perror("\tNo breakpoints."), -1;

	int pos = check_exist(*list_bp, func_name);
	if(pos == -1) {
		printf("\tThere is no breakpoint for '%s'.\n", func_name);
		return -1;
	}

	unsigned int data = 0;
	func_bp* tmp = *list_bp;

	// Go to the breakpoint to remove
	for (int i = 0; i < pos; ++i)
		tmp = tmp->next;
	//tmp = tmp->next;

	// Get the actual data
	ptrace(PTRACE_PEEKTEXT, child, (void *)tmp->addr, 0);

	// Restore the data
    ptrace(PTRACE_POKETEXT, child, (void *)tmp->addr, (data & 0xFFFFFF00) | (tmp->data & 0xFF));

    // Delete the breakpoint from the list
    if(delete_bp(list_bp, pos) == -1)
        return -1;

    printf("\tBreakpoint %d removed.\n", pos);

    return 0;
}

void list_all_bp(func_bp *list_bp, int count)
{
	printf("\t%d breakpoint(s).\n", count);

	if(!list_bp)
		return;

	func_bp* tmp = list_bp;
	int i = 0;

	printf("\t%d: 0x%lx - %s\n", i, tmp->addr, tmp->name);
	while(tmp->next){
		i++;
		tmp = tmp->next;
		printf("\t%d: 0x%lx - %s\n", i, tmp->addr, tmp->name);
	}
}

void free_list_bp(func_bp **list_bp, int count)
{
	for (int i = count-1; i >= 0; --i)
		delete_bp(list_bp, i);
}