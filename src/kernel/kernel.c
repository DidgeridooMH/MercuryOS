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
  printf("Mercury OS 0.0.2 Alpha\n");

  asm("sti");

  halt();
}
