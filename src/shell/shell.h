#ifndef SHELL_H
#define SHELL_H

int shell_update_buffer(char* buffer, int* index, char key);

void shell_remove_from_buffer(char* buffer, int* index);

void shell_prompt();

void shell_process_command(char* command);

void shell_shutdown();

void program_load_test();

#endif
