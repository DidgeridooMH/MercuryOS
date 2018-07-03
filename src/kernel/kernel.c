void halt() {
  for(;;);
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

  char* command;
  int command_index = 0;

  while(1) {
    if(get_keyboard_pointer() > 0) {
      char keyPress = keyboard_pull();
      vga_putchar(keyPress);

      if(keyPress == '\n') {
        command[command_index++] = '\0';
        shell_process_command(command);
        while(command_index >= 0) {
          command[command_index--] = 0;
        }
        command_index = 0;
      } else {
        command[command_index++] = keyPress;
      }
    }
  }

  halt();
}
