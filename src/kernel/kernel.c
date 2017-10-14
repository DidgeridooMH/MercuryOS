int kmain() {
  vga_init();
  gdt_load();
  printf("Mercury OS 0.0.2 Alpha");

  for(;;);
}
