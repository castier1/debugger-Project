#pragma once


/* ------- CHILD PROCESS ------- */

//
void exec_prog(const char *filename);

// Run the given programm to analyse
int run_prog(const char *filename);



/* ------- PARENT PROCESS ------- */

//
void *open_elf(const char *filename);

//
void get_source_file(const char *filename);

//
void where_am_i(const char *file, const char *function, const int line);

//
void print_pwd(const char *filename);

//
char *print_si_code(const int si_signo, const int si_code);

// Analyse the signal
void getsignal(const pid_t child);

// Print a list of all the options
void helpMsg();

//
void kill_child_process(const pid_t child);

// Resume the execution of the programm to analyse
void resume(const pid_t child);

// Start the analyse (with user's interface)
int start_UI(const pid_t child, const gid_t gid, const char *filename);