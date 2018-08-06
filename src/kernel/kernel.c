void halt() {
  for(;;);
}

int command_prompt() {
  char command[256];
  memset(command, 0, 256);
  int command_index = 0;
  command[0] = '\0';

  while(1){
    if(get_keyboard_pointer() > 0) {
      char keyPress = keyboard_pull();
      vga_putchar(keyPress);

      if(keyPress == '\n') {
        shell_process_command(command);
        break;
      } else {
        if(command_index < 255) {
          command[command_index] = keyPress;
          command_index++;
          command[command_index] = '\0';
        }
      }
    }
  }
}

int kmain() {
  vga_init();
  printf("Video context loaded\n");
  gdt_load();
  printf("GDT set\n");
  idt_load();
  printf("IDT populated\n");
  irq_install();
  printf("IRQ Installed\n");
  timer_install();
  timer_phase(100);
  printf("Timer has been initialized\n");
  keyboard_install();
  printf("Keyboard irq initialized\n");
  vga_clear();
  printf("Mercury OS 0.0.2 Alpha\n\n");

  asm("sti");

  while(1) {
    printf(">");
    command_prompt();
  }

  halt();
}
