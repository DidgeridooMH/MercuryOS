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

  while(1) {
    printf(">");
    shell_prompt();
  }

  halt();
}
