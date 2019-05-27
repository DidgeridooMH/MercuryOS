#ifndef ACPI_H
#define ACPI_H

#define RSDP_START (unsigned int*)0xE0000
#define RSDP_END 0x100000

struct rsdp_descriptor {
    char signature[8];
    unsigned char checksum;
    char OEMID[6];
    unsigned char revision;
    unsigned int rsdt_address;
} __attribute__((packed));

struct acpi_sdt_header {
    char signature[8];
    unsigned int length;
    unsigned char revision;
    unsigned char checksum;
    unsigned char oem_id;
    unsigned char oem_table_id;
    unsigned int oem_revision;
    unsigned int creator_id;
    unsigned int creator_revision;
};

struct rsdp_descriptor* acpi_get_rsd_ptr();

int acpi_check_rsdp_header(void* ptr);

int acpi_verify_rsdp(void* ptr);

int acpi_verify_rsdt(void* ptr);

void acpi_shutdown();

#endif
