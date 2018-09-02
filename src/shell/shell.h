#ifndef SHELL_H
#define SHELL_H
#include "../drivers/io.h"

int shell_update_buffer(char* buffer, int* index, char key, Io* io);

void shell_remove_from_buffer(char* buffer, int* index);

void shell_prompt(Io* io);

void shell_process_command(char* command, Io* io);

void shell_shutdown(Io* io);

#endif
