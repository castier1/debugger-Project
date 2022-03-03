#pragma once

//
void exec_prog(const char *filename);

// Run the given programm to analyse
int run_prog(const char *filename);

//
void print_pwd(char *filename);

//
char *print_si_code(int si_signo, int si_code);

// Analyse the signal
void getsignal(pid_t child);

// Print a list of all the options
void helpMsg();

//
void kill_child_process(pid_t child);

// Resume the execution of the programm to analyse
void resume(pid_t child);

// Start the analyse (with user's interface)
int start_UI(pid_t child, gid_t gid, char *filename);