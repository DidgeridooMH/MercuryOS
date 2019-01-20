#include "acpi.h"
#include "../../drivers/io.h"
#include "../memory/mmu.h"

struct rsdp_descriptor* acpi_get_rsd_ptr() {
    unsigned int* ptr;
    for(ptr = RSDP_START; (unsigned int)ptr < RSDP_END; ptr += 0x10 / sizeof(ptr)) {
        if(acpi_check_rsdp_header(ptr)) {
            return (struct rsdp_descriptor*)ptr;
        }
    }

    map_page((void*)0, (void*)0, 2);
    int ebda = *((short *)0x40E);
    ebda = ebda * 0x10  & 0x000FFFFF;

    for(ptr = (unsigned int*)ebda; (int)ptr < ebda + 0x400; ptr += 0x10 / sizeof(ptr)) {
        if(acpi_check_rsdp_header(ptr)) {
            return (struct rsdp_descriptor*)ptr;
        }
    }
    unmap_page(0);

    return 0;
}

int acpi_check_rsdp_header(void* ptr) {
    const char* signature = "RDS PTR ";

    for(int i = 0; i < 8; i++) {
        if(((char*)ptr)[i] != signature[i]) {
            return 0;
        }
    }

    return 1;
}

void acpi_shutdown() {
    struct rsdp_descriptor* rsd_ptr = acpi_get_rsd_ptr();

    if(rsd_ptr == 0) {
        io_printf("Failed to find RDSP...\n");
    } else {
        io_printf("We found it");
    }
}
