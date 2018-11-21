#include "shell.h"
#include "../kernel/memory/mmu.h"
#include "../runtime/memory.h"
#include "../drivers/keyboard.h"
#include "../runtime/string.h"
#include "../drivers/io.h"
#include "../runtime/itoa.h"

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
  Io::printf(">");

  bool processed = false;
  while(!processed){
    if(get_keyboard_pointer() > 0) {
      char keyPress = keyboard_pull();
      if(keyPress == '\b') {
        if(command_index > 0) {
          Io::putChar(keyPress);
          shell_remove_from_buffer(command, &command_index);
        }
      } else if(command_index < 255) {
        Io::putChar(keyPress);
        if(shell_update_buffer(command, &command_index, keyPress)){
          processed = true;
        }
      } else {
        if(keyPress == '\n') {
            Io::putChar(keyPress);
        }
        if(shell_update_buffer(command, &command_index, keyPress)){
          processed = true;
        }
      }
    }
  }
}

void shell_process_command(char* command) {
    if(strcmp(command, "shutdown") == 0) {
        shell_shutdown();
    } else {
        Io::printf("Unable to execute command ");
        Io::printf(command);
        Io::printf("\n");
    }
}

void shell_shutdown() {
  Io::printf("Shutdown\n");
}
