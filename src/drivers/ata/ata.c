#include "ata.h"
#include "../../kernel/memory/mmu.h"
#include "../io.h"

char *ata_type_to_str(enum AtaDeviceType type) {
    switch (type) {
        case ATADEV_PATAPI:
            return "ATA PATAPI Device";
        case ATADEV_SATAPI:
            return "ATA SATAPI Device";
        case ATADEV_PATA:
            return "ATA PATA Device";
        case ATADEV_SATA:
            return "ATA SATA Device";
        case ATADEV_UNKNOWN:
            return "Unknown ATA Device";
    }

    return "Unknown ATA Device Type";
}

void ata_wait_for_drive_select(struct ata_device *dev) {
    io_inportb(dev->bus_mode + ALT_STATUS_REG);
    io_inportb(dev->bus_mode + ALT_STATUS_REG);
    io_inportb(dev->bus_mode + ALT_STATUS_REG);
    io_inportb(dev->bus_mode + ALT_STATUS_REG);
}

void ata_drive_select(struct ata_device *dev) {
    io_outportb(dev->bus_mode + COMMAND_REG, 0x8);
    io_outportb(dev->bus_mode + DRIVE_SEL_REG, dev->drive_mode);

    ata_wait_for_drive_select(dev);
}

enum AtaDeviceType ata_detect_dev_type(struct ata_device *dev) {
    ata_drive_select(dev);

    unsigned short sig = io_inportb(dev->bus_mode + LBA_MID_REG);
    sig |= io_inportb(dev->bus_mode + LBA_HIGH_REG) << 8;

    switch (sig) {
        case 0xEB14:
            return ATADEV_PATAPI;
        case 0x9669:
            return ATADEV_SATAPI;
        case 0:
            return ATADEV_PATA;
        case 0xc33c:
            return ATADEV_SATA;
    }
    return ATADEV_UNKNOWN;
}

void ata_report_device(struct ata_device *dev) {
    if (dev->bus_mode == PRIMARY_BUS) {
        io_printf("Primary IDE Bus | ");
    } else {
        io_printf("Secondary IDE Bus | ");
    }

    if (dev->drive_mode == PRIMARY_DRIVE) {
        io_printf("Primary Drive: ");
    } else {
        io_printf("Secondary Drive: ");
    }

    io_printf(ata_type_to_str(dev->type));
    io_printf("\n");
}

void ata_report_devices() {
    struct ata_device devices[4];
    devices[0].drive_mode = PRIMARY_DRIVE;
    devices[0].bus_mode = PRIMARY_BUS;
    devices[0].type = ata_detect_dev_type(&devices[0]);

    devices[1].drive_mode = SECONDARY_DRIVE;
    devices[1].bus_mode = PRIMARY_BUS;
    devices[1].type = ata_detect_dev_type(&devices[1]);

    devices[2].drive_mode = PRIMARY_DRIVE;
    devices[2].bus_mode = SECONDARY_BUS;
    devices[2].type = ata_detect_dev_type(&devices[2]);

    devices[3].drive_mode = SECONDARY_DRIVE;
    devices[3].bus_mode = SECONDARY_BUS;
    devices[3].type = ata_detect_dev_type(&devices[3]);

    for (int i = 0; i < 4; i++) {
        ata_report_device(devices + i);
    }
}

int ata_identify_command(struct ata_device *dev, unsigned short *buf) {
    // Select target driver
    ata_drive_select(dev);

    ata_wait_for_drive_select(dev);

    // Set parameters to zero
    for (unsigned short i = SEC_CNT_REG; i <= LBA_HIGH_REG; i++) {
        io_outportb(dev->bus_mode + i, 0);
    }

    // Send IDENTIFY command
    io_outportb(dev->bus_mode + COMMAND_REG, dev->type & 0xFF);

    // Read status port
    unsigned char status = io_inportb(dev->bus_mode + STATUS_REG);
    for (int i = 0; i < 5; i++) {
        status = io_inportb(dev->bus_mode + STATUS_REG);
    }

    if (!status) {
        io_set_color(TTY_COLOR_RED, TTY_COLOR_BLACK);
        io_printf("Drive does not exist...\n");
        io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);
        return -1;
    } else if (status & 1) {
        // ATAPI Devices simply give an error flag on identify
        if (dev->type == ATADEV_PATAPI) {
            io_set_color(TTY_COLOR_LIGHT_GREEN, TTY_COLOR_BLACK);
            io_printf("Drive is transfering over ATAPI\n");
            io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);
            return 0;
        }
    }

    io_set_color(TTY_COLOR_LIGHT_GREEN, TTY_COLOR_BLACK);
    io_printf("Drive is present...\n");
    io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);

    // Poll until busy bit not set
    while (io_inportb(dev->bus_mode + STATUS_REG) & 0x80)
        ;
    asm("pause");

    // Check addressing.
    unsigned char LBAmid = io_inportb(dev->bus_mode + LBA_MID_REG);
    unsigned char LBAhi = io_inportb(dev->bus_mode + LBA_HIGH_REG);
    if (LBAmid || LBAhi) {
        io_printf("Drive is not ATA compatible...\n");
        return -1;
    }

    // Wait for data to be readied
    while (!((status = io_inportb(dev->bus_mode + STATUS_REG)) & 0x9))
        ;

    // Check for an error
    if (status & 1) {
        io_set_color(TTY_COLOR_RED, TTY_COLOR_BLACK);
        io_printf("Drive error...\n");
        io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);
        return -1;
    }

    io_printf("Data is ready for reading...\n");
    for (int i = 0; i < 256; i++) {
        buf[i] = io_inportw(dev->bus_mode);
    }

    return 0;
}

void ata_identify_device(struct ata_device *dev) {
    if (dev->bus_mode == PRIMARY_BUS) {
        io_printf("Primary IDE Bus | ");
    } else {
        io_printf("Secondary IDE Bus | ");
    }

    if (dev->drive_mode == PRIMARY_DRIVE) {
        io_printf("Primary Drive: ");
    } else {
        io_printf("Secondary Drive: ");
    }

    unsigned short *buf = kmalloc(256 * sizeof(unsigned short));
    ata_identify_command(dev, buf);
    kfree(buf);
    io_printf("\n");
}

void ata_identify_devices() {
    struct ata_device devices[4];
    devices[0].drive_mode = PRIMARY_DRIVE;
    devices[0].bus_mode = PRIMARY_BUS;
    devices[0].type = ata_detect_dev_type(&devices[0]);

    devices[1].drive_mode = SECONDARY_DRIVE;
    devices[1].bus_mode = PRIMARY_BUS;
    devices[1].type = ata_detect_dev_type(&devices[1]);

    devices[2].drive_mode = PRIMARY_DRIVE;
    devices[2].bus_mode = SECONDARY_BUS;
    devices[2].type = ata_detect_dev_type(&devices[2]);

    devices[3].drive_mode = SECONDARY_DRIVE;
    devices[3].bus_mode = SECONDARY_BUS;
    devices[3].type = ata_detect_dev_type(&devices[3]);

    for (int i = 0; i < 4; i++) {
        ata_identify_device(devices + i);
    }
}
