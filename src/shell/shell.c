#include "shell.h"

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
