#include "shell.h"
#include "../drivers/ata/ata.h"
#include "../drivers/ata/atapi.h"
#include "../drivers/io.h"
#include "../drivers/keyboard.h"
#include "../kernel/acpi/acpi.h"
#include "../kernel/memory/mmu.h"
#include "../runtime/itoa.h"
#include "../runtime/memory.h"
#include "../runtime/string.h"

void shell_remove_from_buffer(char *buffer, int *index) {
    index[0]--;
    buffer[*index] = 0;
}

int shell_update_buffer(char *buffer, int *index, char key) {
    if (key == '\n') {
        shell_process_command(buffer);
        return 1;
    }

    if (*index < 255) {
        buffer[*index] = key;
        index[0]++;
        buffer[*index] = '\0';
    }

    return 0;
}

void shell_prompt() {
    char command[256];
    memset(command, 0, 256);
    int command_index = 0;
    command[0] = '\0';
    io_printf("$>");

    int processed = 0;
    while (!processed) {
        if (keyboard_get_pointer() > 0) {
            char keyPress = keyboard_pull();
            if (keyPress == '\b') {
                if (command_index > 0) {
                    io_put_char(keyPress);
                    shell_remove_from_buffer(command, &command_index);
                }
            } else if (command_index < 255) {
                io_put_char(keyPress);
                if (shell_update_buffer(command, &command_index, keyPress)) {
                    processed = 1;
                }
            } else {
                if (keyPress == '\n') {
                    io_put_char(keyPress);
                }
                if (shell_update_buffer(command, &command_index, keyPress)) {
                    processed = 1;
                }
            }
        }
    }
}

/*
 * :BUG: When deallocating larger than page memory locations when other memory
 * is allocated.
 */
void program_load_test() {
    struct ata_device device;
    device.bus_mode = PRIMARY_BUS;
    device.drive_mode = SECONDARY_DRIVE;
    device.type = ata_detect_dev_type(&device);

    unsigned char *buf = kmalloc(ATAPI_SECTOR_SIZE);
    atapi_read_sector(&device, 0, buf);

    for (int i = 0; i < 64; i++) {
        int cur = buf[i];
        io_printf("%x ", cur);
        if (i % 16 == 15) {
            io_printf("\n");
        }
    }

    kfree(buf);
}

void shell_process_command(char *command) {
    if (strcmp(command, "shutdown") == 0) {
        shell_shutdown();
    } else if (strcmp(command, "test") == 0) {
        program_load_test();
    } else if (strcmp(command, "drive info") == 0) {
        ata_report_devices();
    } else if (strcmp(command, "drive identify") == 0) {
        ata_identify_devices();
    } else {
        io_printf("Unable to execute command ");
        io_printf(command);
        io_printf("\n");
    }
}

void shell_shutdown() { acpi_shutdown(); }
