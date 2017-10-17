int kmain() {
  vga_init();
  printf("Video context loaded\n");
  gdt_load();
  printf("GDT set\n");
  idt_load();
  printf("IDT populated\n");
  printf("Mercury OS 0.0.2 Alpha");

  for(;;);
}
