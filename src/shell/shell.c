#include "shell.h"

void shell_remove_from_buffer(char* buffer, int* index) {
  index[0]--;
  buffer[*index] = 0;
}

int shell_update_buffer(char* buffer, int* index, char key) {
  if(key == '\n') {
    shell_process_command(buffer);
    return 1;
  }

  if(*index < 255) {
    buffer[*index] = key;
    index[0]++;
    buffer[*index] = '\0';
  }

  return 0;
}

void shell_prompt() {
  char command[256];
  memset(command, 0, 256);
  int command_index = 0;
  command[0] = '\0';

  while(1){
    if(get_keyboard_pointer() > 0) {
      char keyPress = keyboard_pull();
      if(command_index > 0 && keyPress == '\b') {
        vga_putchar(keyPress);
        shell_remove_from_buffer(command, &command_index);
      } else if(command_index < 255) {
        vga_putchar(keyPress);
        if(shell_update_buffer(command, &command_index, keyPress) == 1){
          break;
        }
      } else {
        if(shell_update_buffer(command, &command_index, keyPress)){
          break;
        }
      }
    }
  }
}

void shell_process_command(char* command) {
  if(strcmp(command, "shutdown") == 0) {
    shell_shutdown();
  } else {
    printf("Unable to execute command ");
    printf(command);
    printf("\n");
  }
}

void shell_shutdown() {
  printf("Shutdown\n");
}
