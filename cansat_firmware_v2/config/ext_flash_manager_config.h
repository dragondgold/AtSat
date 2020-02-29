#ifndef __EXT_FLASH_MANAGER_CONFIG_H__
#define __EXT_FLASH_MANAGER_CONFIG_H__

// General configuration
#define EXT_FLASH_MANAGER_LFS_STARTING_BLOCK        257

// LittleFS config
#define EXT_FLASH_MANAGER_LFS_READ_SIZE             256
#define EXT_FLASH_MANAGER_LFS_PROG_SIZE             256
#define EXT_FLASH_MANAGER_LFS_BLOCK_SIZE            4096
#define EXT_FLASH_MANAGER_BLOCK_COUNT               1791
#define EXT_FLASH_MANAGER_BLOCK_CYCLES              10000
#define EXT_FLASH_MANAGER_LFS_CACHE_SIZE            256
#define EXT_FLASH_MANAGER_LFS_LOOK_AHEAD_SIZE       256
#define EXT_FLASH_MANAGER_LFS_NAME_MAX              32

// Files
#define EXT_FLASH_MANAGER_LOG_FILENAME              "log.txt"
#define EXT_FLASH_MANAGER_OLD_LOG_FILENAME          "log.old.txt"
#define EXT_FLASH_MANAGER_SETTINGS_FILENAME         "settings"
#define EXT_FLASH_MANAGER_DATA_FILENAME             "data"

// Samples data configuration
#define EXT_FLASH_MANAGER_DATA_SIZE                 80

#endif