#include "acpi.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/string.h"
#include "../memory/mmu.h"

struct rsdp_descriptor* acpi_get_rsd_ptr() {
    map_page_pse((void*)0, (void*)0, 2);
    unsigned int* ptr;
    for (ptr = RSDP_START; (unsigned int)ptr < RSDP_END;
         ptr += 0x10 / sizeof(ptr)) {
        if (acpi_check_rsdp_header(ptr)) {
            return (struct rsdp_descriptor*)ptr;
        }
    }

    int ebda = *((short*)0x40E);
    ebda = ebda * 0x10 & 0x000FFFFF;

    for (ptr = (unsigned int*)ebda; (int)ptr < ebda + 0x400;
         ptr += 0x10 / sizeof(ptr)) {
        if (acpi_check_rsdp_header(ptr)) {
            return (struct rsdp_descriptor*)ptr;
        }
    }

    unmap_page_pse(0);

    return 0;
}

int acpi_verify_rsdp(void* ptr) {
    unsigned char* _ptr = (unsigned char*)ptr;

    unsigned char checksum = 0;

    for (int i = 0; i < sizeof(struct rsdp_descriptor); i++) {
        checksum += _ptr[i];
    }

    return !(checksum);
}

int acpi_check_rsdp_header(void* ptr) {
    struct rsdp_descriptor* _ptr = ptr;
    const char* signature = "RSD PTR ";

    for (int i = 0; i < 8; i++) {
        if (_ptr->signature[i] != signature[i]) {
            return 0;
        }
    }

    if (!acpi_verify_rsdp(ptr)) {
        return 0;
    }

    return 1;
}

int acpi_verify_rsdt(void* ptr) {
    unsigned char* _ptr = (unsigned char*)ptr;

    unsigned char checksum = 0;

    char buf[16];
    itoa(buf, (unsigned int)_ptr, 16);
    io_printf(buf);
    io_printf("\n");

    itoa(buf, (unsigned int)((struct acpi_sdt_header*)ptr)->length, 16);
    io_printf(buf);
    io_printf("\n");

    for (int i = 0; i < ((struct acpi_sdt_header*)ptr)->length; i++) {
        checksum += _ptr[i];
    }

    return !(checksum);
}

void acpi_shutdown() {
    struct rsdp_descriptor* rsd_ptr = acpi_get_rsd_ptr();

    if (rsd_ptr == 0) {
        io_printf("Failed to find RSDP...\n");
        return;
    }

    char buf[16];
    itoa(buf, rsd_ptr, 16);
    io_printf(buf);
    io_printf("\n");
    asm("xchg bx, bx");

    unsigned int table_page = 0x400000 * (rsd_ptr->rsdt_address / 0x400000);
    unsigned int rsdt_length =
        ((struct acpi_sdt_header*)(rsd_ptr->rsdt_address))->length;

    for (int i = 0; i * 0x400000 < rsdt_length + 0x000000; i++) {
        map_page_pse((unsigned int*)(table_page + i * 0x400000),
                     (unsigned int*)(table_page + i * 0x400000), 2);
    }

    asm("xchg bx, bx");
    if (acpi_verify_rsdt((unsigned int*)rsd_ptr->rsdt_address)) {
        io_printf("Verification of rsdt failed...\n");
        return;
    }

    for (int i = 0; i * 0x400000 < rsdt_length + 0x400000; i++) {
        unmap_page_pse(table_page + i * 0x400000);
    }
}
