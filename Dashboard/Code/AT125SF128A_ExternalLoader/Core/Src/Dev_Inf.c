/*
 * Dev_Inf.c
 *
 */
#include "Dev_Inf.h"
#include "at25sf128a.h"
#include "quadspi.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo const StorageInfo = {
#endif
    "AT25SF128A",                  // Device Name + version number
    NOR_FLASH,                           // Device Type
    0x90000000,                          // Device Start Address
    AT25SF128A_FLASH_SIZE,                   // Device Size in Bytes
	AT25SF128A_PAGE_SIZE,                    // Programming Page Size
    0xFF,                                // Initial Content of Erased Memory

    // Specify Size and Address of Sectors (view example below)
    {   {
            (AT25SF128A_FLASH_SIZE / AT25SF128A_SECTOR_SIZE),  // Sector Numbers,
            (uint32_t) AT25SF128A_SECTOR_SIZE
        },       //Sector Size

        { 0x00000000, 0x00000000 }
    }
};
