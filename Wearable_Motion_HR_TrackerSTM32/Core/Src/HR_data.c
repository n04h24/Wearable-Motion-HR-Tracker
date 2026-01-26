/*
 * HR_data.c
 *
 *  Created on: Jan 19, 2026
 *      Author: noah
 */

/* Includes */
#include "HR_data.h"

/* Variable definitions */
uint8_t MODBUS_WRITE_BUFFER[RTU_MSG_MAXLENGTH];
uint8_t MODBUS_READ_BUFFER[RTU_MSG_MAXLENGTH];

/* Implementations */
void MAX30102_collect() {
	/* Format message into buffer */
	MODBUS_send_buffer(MODBUS_WRITE_BUFFER, COLLECT_DATA, COLLECT_DATA_REGADD_A,
			COLLECT_DATA_REGADD_B, COLLECT_REG_NUM_START_A, COLLECT_REG_NUM_START_B);
	/* Send "Collect Data"  */
	HAL_StatusTypeDef UART_RET = HAL_UART_Transmit(&huart1, MODBUS_WRITE_BUFFER, RTU_MSG_MAXLENGTH, 100);
	/* Read Response */
	if (UART_RET == HAL_OK) {
		HAL_UART_Receive(&huart1, MODBUS_READ_BUFFER, RTU_MSG_MAXLENGTH, 100);
		/* Match valid bytes (& operation) */
	}
	/* Stop collecting data
	UART_RET = HAL_UART_Transmit(&huart1, MODBUS_WRITE_BUFFER, RTU_MSG_MAXLENGTH, 100);
	if (UART_RET == HAL_OK) {
		HAL_UART_Receive(&huart1, MODBUS_READ_BUFFER, RTU_MSG_MAXLENGTH, 100);
	} */
}

void MAX30102_HR_SPO2() {

}

void MAX30102_DIE_TEMP() {

}

static void MODBUS_send_buffer(uint8_t *buffer, uint8_t funct_code, uint8_t reg_addA,
								uint8_t reg_addB, uint8_t reg_numA, uint8_t reg_numB) {
	/* Memory helper functions */
	uint8_t pointer_MSG = 0;
	uint8_t MACRO_memory_8 = MODBUS_DEV_ADDRESS;
	/* Device Address (8-bit) */
	memcpy(buffer, &MACRO_memory_8, 1);
	pointer_MSG++;
	/* Function Code (8-bit) */
	MACRO_memory_8 = funct_code;
	memcpy(&buffer[pointer_MSG], &MACRO_memory_8, 1);
	pointer_MSG++;
	/* Register Address or Valid Bytes (16-bit) */
		/* Address (Upper) */
	MACRO_memory_8 = reg_addA;
	memcpy(&buffer[pointer_MSG], &MACRO_memory_8, 1);
	pointer_MSG++;
		/* Address (Lower) */
	MACRO_memory_8 = reg_addB;
	memcpy(&buffer[pointer_MSG], &MACRO_memory_8, 1);
	pointer_MSG++;
	/* Register Number or Data (16-bit/N*8-bit) */
		/* Number (Upper) */
	MACRO_memory_8 = reg_numA;
	memcpy(&buffer[pointer_MSG], &MACRO_memory_8, 1);
	pointer_MSG++;
		/* Number (Lower) */
	MACRO_memory_8 = reg_numB;
	memcpy(&buffer[pointer_MSG], &MACRO_memory_8, 1);
	pointer_MSG++;
	/* Calculate CRC */
	uint16_t crc_val = CRC_check(buffer, pointer_MSG); /* Verify pointer */
	/* Append CRC (upper) */
	uint8_t crc_val_A = ((uint16_t)crc_val & (uint16_t)0xFF00) >> 8;
	memcpy(&buffer[pointer_MSG], &crc_val_A, 1);
	pointer_MSG++;
	/* Append CRC (lower) */
	uint8_t crc_val_B = (uint16_t)crc_val & ((uint16_t)0x00FF);
	memcpy(&buffer[pointer_MSG], &crc_val_B, 1);
	pointer_MSG = 0;

}

static uint16_t CRC_check(uint8_t *data, uint8_t len) {

	uint16_t crc = 0xFFFF;
	  for( uint8_t pos = 0; pos < len; pos++)
	  {
	    crc ^= (uint16_t)data[ pos ];
	    for(uint8_t i = 8; i != 0; i--)
	    {
	      if((crc & 0x0001) != 0){
	        crc >>= 1;
	        crc ^= 0xA001;
	      }else{
	        crc >>= 1;
	      }
	    }
	  }
	  crc = ((crc & 0x00FF) << 8) | ((crc & 0xFF00) >> 8);
	  return crc;

}
