#ifndef ATA_H
#define ATA_H

#define ERROR_REG 1
#define FEATURES_REG 1
#define SEC_CNT_REG 2
#define LBA_LOW_REG 3
#define LBA_MID_REG 4
#define LBA_HIGH_REG 5
#define DRIVE_SEL_REG 6
#define STATUS_REG 7
#define COMMAND_REG 7
#define ALT_STATUS_REG 0x206
#define DEV_CTRL_REG 0x206

enum IdeControllerMode { PRIMARY_BUS = 0x1F0, SECONDARY_BUS = 0x170 };

enum DriveSelectMode { PRIMARY_DRIVE = 0xA0, SECONDARY_DRIVE = 0xB0 };

enum DriveCommMode { ATA_COMM_MODE, ATAPI_COMM_MODE };

enum AtaDeviceType {
    ATADEV_PATAPI = 0x1A1,
    ATADEV_SATAPI = 0x2A1,
    ATADEV_PATA = 0x1EC,
    ATADEV_SATA = 0x2EC,
    ATADEV_UNKNOWN
};

struct ata_device {
    enum AtaDeviceType type;
    int drive_mode;
    int bus_mode;
};

void ata_wait_for_drive_select(struct ata_device *dev);

void ata_drive_select(struct ata_device *dev);

int ata_identify_command(struct ata_device *dev, unsigned short *buf);

void ata_report_devices();

void ata_identify_devices();

enum AtaDeviceType ata_detect_dev_type(struct ata_device *dev);

#endif
