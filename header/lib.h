#pragma once

/* ------- CHILD PROCESS ------- */

// Start program execution
void exec_prog(char * const *argv);

// Run the given program and active ptrace
int run_prog(char * const *argv);



/* ------- PARENT PROCESS ------- */

// [NOT-USED-YET] Print the current location of this funcion
void where_am_i(const char *file, const char *function, const int line);

// Print a list of all the options
void helpMsg();

// Kill a given processus
void kill_child_process(const pid_t child, const int status);

// Resume the execution of a processus
void resume(const pid_t child, int *status);

// Start the analyse (with user's interface)
int start_UI(const pid_t child, const int stat, const char *filename);