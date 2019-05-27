<p align=center>
<img src="https://github.com/didgeridoomh/mercuryos/raw/master/resources/mercury_new.png" alt="MercuryOS Logo" width="80%">
</p>

Mercury OS is a simple operating system used to learn about the different concepts of OS design. It is written using C and Assembly and compiled using GCC. The kernel is loaded using GRUB. The objective is to make a lightweight, extensible platform that can run simple applications at high speeds.

## Requirements

### Installing:

- 32-bit Processor or higher

- 1GB of RAM

### Developing:

- GCC configured toolchain

- LD linker

- Nasm

- QEMU or Bochs

## Building

Build a tool chain as described by the OSDev wiki: [OSDev Wiki](http://wiki.osdev.org/Bare_Bones).

To get nasm on linux systems use

```
sudo apt-get install nasm
```

Windows and MacOS sources and binaries are avaliable at [Nasm](http://www.nasm.us/pub/nasm/releasebuilds/?C=M;O=D).

To build use the makefile at the root directory.

Qemu can be used by executing

```
make run-qemu
```

Bochs is recommended for debugging and can be used with

```
make run-bochs
```

## Features

- [x] Bootable image

- [x] Basic terminal output

- [x] GDT implementation

- [x] IRS implementation

- [x] IRQ implementation

- [x] Basic keyboard driver

- [x] Basic Timer

- [x] Refine keyboard driver

- [ ] Extend and refine I/O drivers (Centralized Input and Output Function)

- [ ] Userland shell

- [ ] Filesystem support

- [ ] Load ELF Executable
