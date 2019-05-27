#include "atapi.h"
#include "../io.h"

int atapi_read_sector(struct ata_device *dev, unsigned int lba,
                      unsigned char *buf) {
    /* 0xA8 is the "Read Sectors" command byte. */
    unsigned char read_cmd[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ata_drive_select(dev);
    /* Set PIO Mode. */
    io_outportb(dev->bus_mode + FEATURES_REG, 0x0);
    io_outportb(dev->bus_mode + LBA_MID_REG, ATAPI_SECTOR_SIZE & 0xFF);
    io_outportb(dev->bus_mode + LBA_HIGH_REG, ATAPI_SECTOR_SIZE >> 8);
    /* ATA PACKET command */
    io_outportb(dev->bus_mode + COMMAND_REG, 0xA0);

    unsigned char status;
    /* Wait for busy signal to go away. */
    while ((status = io_inportb(dev->bus_mode + STATUS_REG)) & 0x80) {
        asm("pause");
    }

    /* Wait for either DRQ or Error flag. */
    while (!((status = io_inportb(dev->bus_mode + STATUS_REG)) & 0x9)) {
        asm("pause");
    }

    /* Error flag was set. */
    if (status & 0x1) {
        io_set_color(TTY_COLOR_RED, TTY_COLOR_BLACK);
        io_printf("Error while reading from disc...STATUS: %x",
                  (unsigned int)status);
        io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);
        return -1;
    }

    /* Read only one sector */
    read_cmd[9] = 1;

    /* Load in Big Endian order */
    read_cmd[2] = (lba >> 0x18) & 0xFF;
    read_cmd[3] = (lba >> 0x10) & 0xFF;
    read_cmd[4] = (lba >> 0x08) & 0xFF;
    read_cmd[5] = (lba >> 0x00) & 0xFF;

    /* Send the command */
    io_outportsw(dev->bus_mode, (unsigned char *)read_cmd, 6);

    /* TODO: Multitask by using IRQs */
    /* Wait for busy signal to go away. */
    while ((status = io_inportb(dev->bus_mode + STATUS_REG)) & 0x80) {
        asm("pause");
    }

    /* Calculate size */
    int size = (((int)io_inportb(dev->bus_mode + LBA_HIGH_REG))) << 8 |
               (int)(io_inportb(dev->bus_mode + LBA_MID_REG));

    /* Assert that size is full size of request */
    if (size != ATAPI_SECTOR_SIZE) {
        io_set_color(TTY_COLOR_RED, TTY_COLOR_BLACK);
        io_printf("Disc error...SIZE MISMATCH: %x expected %x",
                  (unsigned int)size, ATAPI_SECTOR_SIZE);
        io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);
        return -1;
    }

    /* Read data */
    io_inportsw(dev->bus_mode, buf, size / 2);

    /* Usually another IRQ comes through */

    /* Wait for busy and DRQ to clear */
    while ((status = io_inportb(dev->bus_mode + COMMAND_REG)) & 0x88) {
        asm("pause");
    }

    return size;
}
