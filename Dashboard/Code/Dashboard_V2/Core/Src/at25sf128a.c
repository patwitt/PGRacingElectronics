/*
 * at25sf128a.c
 *
 *  Created on: 20 lut 2022
 *      Author: Karol
 */

#include "at25sf128a.h"
#include "quadspi.h"
/* USER CODE BEGIN 0 */
static uint8_t FLASH_QSPI_WriteEnable(void);
static uint8_t FLASH_QSPI_AutoPollingMemReady(void);
static uint8_t FLASH_QSPI_Configuration(void);
static uint8_t FLASH_QSPI_ResetChip(void);
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
uint8_t FLASH_QSPI_Init(void) {
	//prepare QSPI peripheral for ST-Link Utility operations
	hqspi.Instance = QUADSPI;
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK) {
		return HAL_ERROR;
	}

	MX_QUADSPI_Init();

	if (FLASH_QSPI_ResetChip() != HAL_OK) {
		return HAL_ERROR;
	}

	HAL_Delay(1);

	if (FLASH_QSPI_AutoPollingMemReady() != HAL_OK) {
		return HAL_ERROR;
	}

	if (FLASH_QSPI_WriteEnable() != HAL_OK) {

		return HAL_ERROR;
	}

	/* Sets QE bit and starts QUADSPI communication */
	if (FLASH_QSPI_Configuration() != HAL_OK) {
		return HAL_ERROR;
	}

	if (FLASH_QSPI_EnableMemoryMappedMode() != HAL_OK) {
			return HAL_ERROR;
		}

	return HAL_OK;
}

uint8_t FLASH_QSPI_EraseChip(void) {
	QSPI_CommandTypeDef sCommand;

	if (FLASH_QSPI_WriteEnable() != HAL_OK) {
		return HAL_ERROR;
	}

	/* Erasing Sequence --------------------------------- */
	sCommand.Instruction = CHIP_ERASE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	if (FLASH_QSPI_AutoPollingMemReady() != HAL_OK) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

uint8_t FLASH_QSPI_EraseSector(uint32_t EraseStartAddress,
		uint32_t EraseEndAddress) {

	QSPI_CommandTypeDef sCommand;

	EraseStartAddress = EraseStartAddress
			- EraseStartAddress % AT25SF128A_SECTOR_SIZE;

	/* Erasing Sequence -------------------------------------------------- */
	sCommand.Instruction = BLOCK_ERASE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;

	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	while (EraseEndAddress >= EraseStartAddress) {
		sCommand.Address = (EraseStartAddress & 0x0FFFFFFF);

		if (FLASH_QSPI_WriteEnable() != HAL_OK) {
			return HAL_ERROR;
		}

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
				!= HAL_OK) {
			return HAL_ERROR;
		}
		EraseStartAddress += AT25SF128A_SECTOR_SIZE;

		if (FLASH_QSPI_AutoPollingMemReady() != HAL_OK) {
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}

uint8_t FLASH_QSPI_EraseBlock(uint32_t BlockAddress) {
	QSPI_CommandTypeDef s_command;

	/* Initialize the erase command */
	s_command.Instruction = SECTOR_ERASE_CMD;
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_24_BITS;
	s_command.Address = BlockAddress;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_NONE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (FLASH_QSPI_WriteEnable() != HAL_OK) {
		return HAL_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	/* Configure automatic polling mode to wait for end of erase */
	if (FLASH_QSPI_AutoPollingMemReady() != HAL_OK) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

uint8_t FLASH_QSPI_ResetChip() {
	QSPI_CommandTypeDef sCommand;
	uint32_t temp = 0;
	/* Erasing Sequence -------------------------------------------------- */
	sCommand.Instruction = ENABLE_RESET_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}
	for (temp = 0; temp < 0x2f; temp++) {
		__NOP();
	}

	sCommand.Instruction = RESET_DEVICE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint8_t FLASH_QSPI_AutoPollingMemReady(void) {

	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Configure automatic polling mode to wait for memory ready ------ */
	sCommand.Instruction = READ_STATUS_REG_LOWER_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	sConfig.Match = 0x00;
	sConfig.Mask = AT25SF128A_SR_WIP;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

static uint8_t FLASH_QSPI_WriteEnable(void) {
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Enable write operations ------------------------------------------ */
	sCommand.Instruction = WRITE_ENABLE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	/* Configure automatic polling mode to wait for write enabling ---- */
	sConfig.Match = 0x02;
	sConfig.Mask = 0x02;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
	sCommand.Instruction = READ_STATUS_REG_LOWER_CMD;
	sCommand.DataMode = QSPI_DATA_1_LINE;

	if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

/*Enable quad mode and set dummy cycles count*/
uint8_t FLASH_QSPI_Configuration(void) {

	QSPI_CommandTypeDef sCommand;
	uint8_t reg;

	/*read configuration register*/
	sCommand.Instruction = READ_STATUS_REG_MIDDLE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	if (HAL_QSPI_Receive(&hqspi, &reg,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}

	if (FLASH_QSPI_WriteEnable() != HAL_OK) {

		return HAL_ERROR;
	}

	/*set dummy cycles*/
	//MODIFY_REG(reg, 0xF0, (N25Q128A_DUMMY_CYCLES_READ_QUAD << POSITION_VAL(0xF0)));
	/* Set QE bit in register to enable QUADSPI mode in at25sf128a */
	reg = reg | 0x02;

	sCommand.Instruction = WRITE_STATUS_REG_MIDDLE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	if (HAL_QSPI_Transmit(&hqspi, &reg,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint8_t FLASH_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size) {
	QSPI_CommandTypeDef s_command;
	uint32_t end_addr, current_size, current_addr;

	/* Calculation of the size between the write address and the end of the page */
	current_size = AT25SF128A_SECTOR_SIZE
			- (WriteAddr % AT25SF128A_SECTOR_SIZE);

	/* Check if the size of the data is less than the remaining place in the page */
	if (current_size > Size) {
		current_size = Size;
	}

	/* Initialize the adress variables */
	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;

	/* Initialize the program command */
	s_command.Instruction = QUAD_PAGE_PROGRAM_CMD;
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_4_LINES;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Perform the write page by page */
	do {
		s_command.Address = current_addr;
		s_command.NbData = current_size;

		/* Enable write operations */
		if (FLASH_QSPI_WriteEnable() != HAL_OK) {
			return HAL_ERROR;
		}

		/* Configure the command */
		if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
				!= HAL_OK) {
			return HAL_ERROR;
		}

		/* Transmission of the data */
		if (HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
				!= HAL_OK) {
			return HAL_ERROR;
		}

		/* Configure automatic polling mode to wait for end of program */
		if (FLASH_QSPI_AutoPollingMemReady() != HAL_OK) {
			return HAL_ERROR;
		}

		/* Update the address and size variables for next page programming */
		current_addr += current_size;
		pData += current_size;
		current_size =
				((current_addr + AT25SF128A_PAGE_SIZE) > end_addr) ?
						(end_addr - current_addr) : AT25SF128A_PAGE_SIZE;
	} while (current_addr < end_addr);

	return HAL_OK;
}

uint8_t FLASH_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size) {
	QSPI_CommandTypeDef s_command;

	/* Initialize the read command */
	s_command.Instruction = QUAD_INOUT_FAST_READ_CMD;
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode = QSPI_ADDRESS_4_LINES;
	s_command.AddressSize = QSPI_ADDRESS_24_BITS;
	s_command.Address = ReadAddr;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.AlternateBytes = 0x0;//do poprawy
	s_command.DataMode = QSPI_DATA_4_LINES;
	s_command.DummyCycles = 0x4; // do poprawy przez define
	s_command.NbData = Size;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD; //do zmiany jesli nie działa  usunąć

	      //do poprawy

	/* Configure the command */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	/* Set S# timing for Read command */
	MODIFY_REG(hqspi.Instance->DCR, QUADSPI_DCR_CSHT,
			QSPI_CS_HIGH_TIME_3_CYCLE);

	/* Reception of the data */
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	/* Restore S# timing for nonRead commands */
	MODIFY_REG(hqspi.Instance->DCR, QUADSPI_DCR_CSHT,
			QSPI_CS_HIGH_TIME_6_CYCLE);

	return HAL_OK;
}

uint8_t FLASH_QSPI_EnableMemoryMappedMode(void) {

	QSPI_CommandTypeDef s_command;
	QSPI_MemoryMappedTypeDef sMemMappedCfg;

	/* Enable Memory-Mapped mode-------------------------------------------------- */

	s_command.Instruction = QUAD_INOUT_FAST_READ_CMD;
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
		s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
		s_command.AlternateBytes = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode = QSPI_ADDRESS_4_LINES;
	s_command.DataMode = QSPI_DATA_4_LINES;
	s_command.NbData = 0;
	s_command.Address = 0;
	s_command.DummyCycles = 0x4; //moze 4?

	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

	if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &sMemMappedCfg) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_OK;
}
