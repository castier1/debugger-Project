#pragma once

/* ------- CHILD PROCESS ------- */

// Start program execution
void exec_prog(char * const* argv);

// Run the given program and active ptrace
int run_prog(char * const* argv);



/* ------- PARENT PROCESS ------- */

// Get the syscall name
char *syscall_name(long long int id);

// Print the syscall and and print it's description
void print_syscall(const pid_t child, int status, int check_status);

// Jump to the next syscall
int jump_syscall(const pid_t child, int status, int check_status);

// Run the program and print all it's execution
void print_all_syscall(const pid_t child, int status);

// Print the result of all or a part of the objump command
void print_dump(const char *filename, const char *input);

// Print the name of all the file descriptor opened
void print_file_descr(const pid_t child);

// Print all the loaded dynamic librairies
void print_lib(const pid_t child);

// Print the file type
void print_filetype(const mode_t mode);

// Print the permissions
void print_mode(const mode_t mode);

// Print the name of the owner
void print_owner(uid_t uid);

// Print the common metadata
void print_metadata(const char *filename);

// Print the result of the addr2line command
void print_function_infos(const char *filename, const size_t addr);

// Check and open an elf file
void *open_elf(const char *filename);

// Parse an elf file and retrieve data: source file name or functions name
void parse_symtab(const char *filename, const unsigned char TYPE, const char *func_name);

// [NOT-USED-YET] Print the current location of this funcion
void where_am_i(const char *file, const char *function, const int line);

// Print the absolute path of a file
void print_pwd(const char *filename);

// Get the signal description
char *print_si_code(const int si_signo, const int si_code);

// Analyse the signal and print it's description
void getsignal(const pid_t child);

// Print a list of all the options
void helpMsg();

// Kill a given processus
void kill_child_process(const pid_t child);

// Resume the execution of a processus
void resume(const pid_t child);

// Start the analyse (with user's interface)
int start_UI(const pid_t child, int stat, const char *filename);