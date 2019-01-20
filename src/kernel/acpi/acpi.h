#ifndef ACPI_H
#define ACPI_H

#define RSDP_START (unsigned int*)0xE0000
#define RSDP_END   0x10000

struct rsdp_descriptor {
    char signature[8];
    unsigned char checksum;
    char OEMID[6];
    unsigned char revision;
    unsigned int rsdt_address;
} __attribute__ ((packed));

struct rsdp_descriptor* acpi_get_rsd_ptr();

int acpi_check_rsdp_header(void* ptr);

void acpi_shutdown();

#endif
