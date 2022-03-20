#pragma once

// Get the signal description
char *print_si_code(const int si_signo, const int si_code);

// Analyse the signal and print it's description
void getsignal(const pid_t child, const int status);