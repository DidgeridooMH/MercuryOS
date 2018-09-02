#include "../drivers/vga.h"
#include "arch/x86.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "system_calls.h"
#include "timer.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"
#include "../runtime/itoa.h"

extern "C" int kmain() {
  vga_init();
  printf("Video context loaded\n");

  gdt_load();
  printf("GDT set\n");

  idt_load();
  printf("IDT populated\n");

  system_calls_install();
  printf("System Calls Installed\n");

  timer_install();
  timer_phase(100);
  printf("Timer has been initialized\n");

  keyboard_install();
  printf("Keyboard irq initialized\n");

  vga_clear();
  printf("Mercury OS 0.0.2 Alpha\n\n");

  asm("sti");

  while(1) {
    shell_prompt();
  }

  return 0;
}
