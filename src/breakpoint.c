#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>

#include "../header/process.h"
#include "../header/binary.h"
#include "../header/breakpoint.h"

int check_exist(func_bp *list_bp, const char *func_name)
{
	int i = 0;
	func_bp *tmp = list_bp;

	// Parse all the list
	while(tmp)
	{
		// Return position in list
		if(strcmp(tmp->name, func_name) == 0)
			return i;

		// Go to the next breakpoint
		tmp = tmp->next;
		i++;
	}

	// If not found
	return -1;
}

void add_bt(const pid_t child, func_bp *bp)
{
	// Save the original data
	bp->data = ptrace(PTRACE_PEEKDATA, child, (void *)bp->addr, 0);
	if(bp->data == -1)
		perror("\tERROR : add_bt : PTRACE_PEEKTEXT");

	// Add the breakpoint
	long data = (bp->data & 0xffffff00) | 0xcc;
    if(ptrace(PTRACE_POKEDATA, child, (void *)bp->addr, data) == -1)
    	perror("\tERROR : add_bt : PTRACE_POKETEXT");

    printf("\tBreakpoint added at the %s function, at the 0x%lx address.\n", bp->name, bp->addr);
}

int create_bp(const pid_t child, const int status, func_bp **list_bp, const char *func_name, const char *filename)
{
	// Check if the process is already stopped
    if(WIFEXITED(status)){
        printf("\tProcess %d stopped.\n", child);
        return -1;
    }
    // Check if it already exist a breakpoint for this function
    if(check_exist(*list_bp, func_name) != -1){
		printf("\tAlready exist a breakpoint for '%s'.\n", func_name);
		return -1;
	}

	long int addr = 0;
	// If no address but the name of the function
	if(func_name){
		addr = parse_symtab(filename, STT_FUNC, func_name, 1);
		addr += get_start_stack_addr(child);
	}
	// If still no adress
	if(!addr){
		printf("\tCannot find right address for '%s'\n", func_name);
		return -1;
	}

	// Go to the last breakpoint
	func_bp *tmp = *list_bp;
	while(tmp && tmp->next)
		tmp = tmp->next;

	// Create and stock breakpoint's data
	func_bp *new_bp = malloc(sizeof(*new_bp));
	if(!new_bp)
		return perror("\tERROR : create_bp : malloc"), -1;

    strcpy(new_bp->name, func_name);
    new_bp->addr = addr;
    new_bp->next = NULL;

    if(tmp)
	    tmp->next = new_bp;
	else
	    *list_bp = new_bp;

	// Add the breakpoint in the process
    add_bt(child, new_bp);

    return 0;
}

int delete_bp(func_bp **list_bp, const int pos)
{
	// If empy list, can't remove
	if(!*list_bp)
		return -1;

	func_bp* tmp = *list_bp;
	func_bp *old_tmp;

	// If it's at first position
	if (pos == 0)
		*list_bp = (*list_bp)->next;
	// If that's further in the list
	else
	{
		// Go to the breakpoint just before
		for (int i = 1; i < pos-1; ++i)
			tmp = tmp->next;

		// Connect the one before with the one after,
		// to remove it from the list
		old_tmp = tmp;
		tmp = tmp->next;
		if(!tmp->next)
		     old_tmp->next = NULL;
		else
		    old_tmp->next = tmp->next;
	}

	// Delete the breakpoint
	free(tmp);
}

int remove_bp(const pid_t child, const int status, func_bp **list_bp, const char *func_name, int pos)
{
	// Check if the process is already stopped
    if(WIFEXITED(status)){
        printf("\tProcess %d stopped.\n", child);
        return -1;
    }

    // If empty list
	if(!*list_bp)
		return perror("\tNo breakpoints."), -1;

	// Find the position in the list of the breakpoint, if no position given
	if(pos == -1) {
		if((pos = check_exist(*list_bp, func_name)) == -1)
		{
			printf("\tThere is no breakpoint for '%s'.\n", func_name);
			return -1;
		}
	}

	func_bp* tmp = *list_bp;

	// Go to the breakpoint to remove
	for (int i = 0; i < pos; ++i)
		tmp = tmp->next;

	// Restore the original data
    if(ptrace(PTRACE_POKEDATA, child, (void *)tmp->addr, tmp->data) == -1)
    	perror("\tERROR : remove_bp : PTRACE_POKETEXT");

    // Delete the breakpoint from the list
    if(delete_bp(list_bp, pos) == -1)
        return -1;

    printf("\tBreakpoint %d removed.\n", pos);
    return 0;
}

void list_all_bp(func_bp *list_bp, const int count)
{
	printf("\t%d breakpoint(s).\n", count);

	// If list empty
	if(!list_bp)
		return;

	func_bp* tmp = list_bp;
	int i = 0;

	// Parse the list and print the data
	printf("\t%d: 0x%lx - %s\n", i, tmp->addr, tmp->name);
	while(tmp->next){
		i++;
		tmp = tmp->next;
		printf("\t%d: 0x%lx - %s\n", i, tmp->addr, tmp->name);
	}
}

void free_list_bp(const pid_t child, const int status, func_bp **list_bp, const int count)
{
	// Parse the list
	for (int i = count-1; i >= 0; --i)
		remove_bp(child, status, list_bp, NULL, i);
}