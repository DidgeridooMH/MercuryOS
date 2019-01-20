#include "shell.h"
#include "../kernel/memory/mmu.h"
#include "../runtime/memory.h"
#include "../drivers/keyboard.h"
#include "../runtime/string.h"
#include "../drivers/io.h"
#include "../runtime/itoa.h"
#include "../kernel/acpi/acpi.h"

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
  io_printf(">");

  int processed = 0;
  while(!processed){
    if(keyboard_get_pointer() > 0) {
      char keyPress = keyboard_pull();
      if(keyPress == '\b') {
        if(command_index > 0) {
          io_put_char(keyPress);
          shell_remove_from_buffer(command, &command_index);
        }
      } else if(command_index < 255) {
        io_put_char(keyPress);
        if(shell_update_buffer(command, &command_index, keyPress)){
          processed = 1;
        }
      } else {
        if(keyPress == '\n') {
            io_put_char(keyPress);
        }
        if(shell_update_buffer(command, &command_index, keyPress)){
          processed = 1;
        }
      }
    }
  }
}

void program_load_test() {
    // unsigned char shell_code[11] = { 0xB8, 0x00, 0x80, 0x0B, 0x00,
    //     0x66, 0xC7, 0x00, 0x48, 0xFF, 0xC3 };
    //
    // unsigned char* program_block = (unsigned char*)kmalloc(11);
    //
    // for (int i = 0; i < 11; i++) {
    //     program_block[i] = shell_code[i];
    // }
    //
    // ((void(*)())program_block)();
    //
    // deallocate_memory(program_block);

}

void shell_process_command(char* command) {
    if(strcmp(command, "shutdown") == 0) {
        shell_shutdown();
    } else if(strcmp(command, "test") == 0) {
        program_load_test();
    } else {
        io_printf("Unable to execute command ");
        io_printf(command);
        io_printf("\n");
    }
}

void shell_shutdown() {
    acpi_shutdown();
}
