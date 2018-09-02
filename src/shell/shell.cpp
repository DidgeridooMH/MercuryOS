#include "shell.h"
#include "../runtime/memory.h"
#include "../drivers/keyboard.h"
#include "../runtime/string.h"

void shell_remove_from_buffer(char* buffer, int* index) {
  index[0]--;
  buffer[*index] = 0;
}

int shell_update_buffer(char* buffer, int* index, char key, Io* io) {
  if(key == '\n') {
    shell_process_command(buffer, io);
    return 1;
  }

  if(*index < 255) {
    buffer[*index] = key;
    index[0]++;
    buffer[*index] = '\0';
  }

  return 0;
}

void shell_prompt(Io* io) {
  char command[256];
  memset(command, 0, 256);
  int command_index = 0;
  command[0] = '\0';
  io->printf(">");

  while(1){
    if(get_keyboard_pointer() > 0) {
      char keyPress = keyboard_pull();
      if(keyPress == '\b') {
        if(command_index > 0) {
          io->putChar(keyPress);
          shell_remove_from_buffer(command, &command_index);
        }
      } else if(command_index < 255) {
        io->putChar(keyPress);
        if(shell_update_buffer(command, &command_index, keyPress, io) == 1){
          break;
        }
      } else {
        if(shell_update_buffer(command, &command_index, keyPress, io)){
          break;
        }
      }
    }
  }
}

void shell_process_command(char* command, Io* io) {
  if(strcmp(command, "shutdown") == 0) {
    shell_shutdown(io);
  } else {
    io->printf("Unable to execute command ");
    io->printf(command);
    io->printf("\n");
  }
}

void shell_shutdown(Io* io) {
  io->printf("Shutdown\n");
}
