#include "shell.h"
#include "../kernel/memory/heap_allocator.h"
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
          Io::put_char(keyPress);
          shell_remove_from_buffer(command, &command_index);
        }
      } else if(command_index < 255) {
        Io::put_char(keyPress);
        if(shell_update_buffer(command, &command_index, keyPress)){
          processed = true;
        }
      } else {
        if(keyPress == '\n') {
            Io::put_char(keyPress);
        }
        if(shell_update_buffer(command, &command_index, keyPress)){
          processed = true;
        }
      }
    }
  }
}

void program_load_test() {
    MMU::map_page((unsigned int*)0x1000000, (unsigned int*)0xF00000, 2);

    Heap::create_frame(0, 0xF00000, 0x1000);

    unsigned char shell_code[11] = { 0xB8, 0x00, 0x80, 0x0B, 0x00,
        0x66, 0xC7, 0x00, 0x48, 0xFF, 0xC3 };

    unsigned char* program_block = (unsigned char*)Heap::allocate_memory(0, 11);

    unsigned char* another_ptr = (unsigned char*)Heap::allocate_memory(0, 256);

    unsigned char* anoter_ptr = (unsigned char*)Heap::allocate_memory(0, 256);

    for (int i = 0; i < 11; i++) {
        program_block[i] = shell_code[i];
    }

    ((void(*)())program_block)();

    Heap::deallocate_memory(anoter_ptr);
    Heap::deallocate_memory(program_block);
    Heap::deallocate_memory(another_ptr);

    Heap::delete_frame(0);

    MMU::unmap_page((unsigned int*)0xF00000);
}

void shell_process_command(char* command) {
    if(strcmp(command, "shutdown") == 0) {
        shell_shutdown();
    } else if(strcmp(command, "test") == 0) {
        program_load_test();
    } else {
        Io::printf("Unable to execute command ");
        Io::printf(command);
        Io::printf("\n");
    }
}

void shell_shutdown() {
  Io::printf("Shutdown\n");
}
