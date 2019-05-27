#ifndef ATAPI_H
#define ATAPI_H

#include "ata.h"

#define ATAPI_SECTOR_SIZE 0x800

int atapi_read_sector(struct ata_device *dev, unsigned int lba,
                      unsigned char *buf);

#endif
