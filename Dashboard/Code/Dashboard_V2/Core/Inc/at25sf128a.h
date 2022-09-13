/*
 * at25sf128a.h
 *
 *  Created on: 20 lut 2022
 *      Author: Karol Pergol
 */

#ifndef INC_AT25SF128A_H_
#define INC_AT25SF128A_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/* AT25ST128A Configuration and commands */

/* AT25ST128A Configuration */

#define AT25SF128A_FLASH_SIZE                  0x1000000 /* 128 MBits => 16MBytes */
#define AT25SF128A_SECTOR_SIZE                 0x10000   /* 256 sectors of 64KBytes */
#define AT25SF128A_SUBSECTOR_SIZE              0x1000    /* 4096 subsectors of 4kBytes */
#define AT25SF128A_PAGE_SIZE                   0x100     /* 65536 pages of 256 bytes */

#define AT25SF128A_DUMMY_CYCLES_READ           4
#define AT25SF128A_DUMMY_CYCLES_READ_QUAD      4

#define AT25SF128A_BULK_ERASE_MAX_TIME         250000
#define AT25SF128A_SECTOR_ERASE_MAX_TIME       3000
#define AT25SF128A_SUBSECTOR_ERASE_MAX_TIME    800

#define AT25SF128A_SR_WIP                      ((uint8_t)0x01)

/* AT25ST128A Commands */

/* Reset Operations */
#define ENABLE_RESET_CMD 0x66
#define RESET_DEVICE_CMD 0x99

/* Register Operations */
#define READ_STATUS_REG_LOWER_CMD 0x05
#define READ_STATUS_REG_MIDDLE_CMD 0x35
#define READ_STATUS_REG_UPPER_CMD 0x15

#define WRITE_STATUS_REG_LOWER_CMD 0x01
#define WRITE_STATUS_REG_MIDDLE_CMD 0x31
#define WRITE_STATUS_REG_UPPER_CMD 0x11

/* Erase Operations */
#define	SECTOR_ERASE_CMD 0x20
#define BLOCK_ERASE_CMD 0xD8
#define HALF_BLOCK_ERASE_CMD 0x52
#define CHIP_ERASE_CMD 0xC7
#define CHIP_ERASE_2_CMD 0x60

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_SUSPEND_CMD               0x75

/* Write Operations */
#define WRITE_ENABLE_CMD 0x06
#define WRITE_DISABLE_CMD 0x04

/* Program Operations */
#define PAGE_PROGRAM_CMD 0x02
#define QUAD_PAGE_PROGRAM_CMD 0x32
#define FAST_PAGE_PROGRAM_CMD 0xF2

/* Read Operations */
#define READ_CMD                             0x03
#define FAST_READ_CMD                        0x0B
#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_INOUT_FAST_READ_CMD             0xEB

/* Identification Operations*/

#define READ_MID_DID_CMD 0x90
#define READ_MID_DID_CMD_DUAL 0x92
#define READ_MID_DID_CMD_QUAD 0x94
#define READ_JEDEC_ID_CMD 0x9F
#define READ_UID_CMD 0x4B

extern QSPI_HandleTypeDef hqspi;

/* Default QUADSPI initializer */
void MX_QUADSPI_Init(void);

/* USER CODE BEGIN Prototypes */

uint8_t FLASH_QSPI_Init(void);
uint8_t FLASH_QSPI_EraseChip(void);
uint8_t FLASH_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
uint8_t FLASH_QSPI_EraseBlock(uint32_t BlockAddress);

/* Read write buffer */
uint8_t FLASH_QSPI_Write(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
uint8_t FLASH_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);

uint8_t FLASH_QSPI_EnableMemoryMappedMode(void);

/* USER CODE END Prototypes */


#ifdef __cplusplus
}
#endif

#endif /* INC_AT25SF128A_H_ */

