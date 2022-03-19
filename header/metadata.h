#pragma once

// Print the file type
void print_filetype(const mode_t mode);

// Print the permissions
void print_mode(const mode_t mode);

// Print the name of the owner
void print_owner(uid_t uid);

// Print the common metadata
void print_metadata(const char *filename);

// Print the absolute path of a file
void print_pwd(const char *filename);