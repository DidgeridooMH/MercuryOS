#include "shell.h"
#include "../kernel/memory/mmu.h"
#include "../runtime/memory.h"
#include "../drivers/keyboard.h"
#include "../runtime/string.h"
#include "../drivers/io.h"
#include "../drivers/ata/ata.h"
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
  io_printf("$>");

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

void shell_drive_info() {
    ata_report_devices();
}

void program_load_test() {
    // char prog[6] = { 0xB8, 0x02, 0x00, 0x00, 0x00, 0xC3 };
    // void *block = kmalloc(sizeof(prog));
    // memcpy(block, prog, sizeof(prog));
    //
    // int i = ((int(*)())block)();
    // if(i == 2) {
    //     io_printf("YEEAASSS\n");
    // }
    //
    // kfree(block);
    // ata_identify_command();
}

void shell_process_command(char* command) {
    if(strcmp(command, "shutdown") == 0) {
        shell_shutdown();
    } else if(strcmp(command, "test") == 0) {
        program_load_test();
    } else if(strcmp(command, "driveinfo") == 0) {
        shell_drive_info();
    } else {
        io_printf("Unable to execute command ");
        io_printf(command);
        io_printf("\n");
    }
}

void shell_shutdown() {
    acpi_shutdown();
}
