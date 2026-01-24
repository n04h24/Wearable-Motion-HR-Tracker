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
	/* Collect Data  */
	HAL_StatusTypeDef UART_RET = HAL_UART_Transmit(&huart1, MODBUS_WRITE_BUFFER, RTU_MSG_MAXLENGTH, 100);
	/* Read Response */
	if (UART_RET == HAL_OK) {
		HAL_UART_Receive(&huart1, MODBUS_READ_BUFFER, RTU_MSG_MAXLENGTH, 100);
		/* Match valid bytes (& operation) */

		/* Pass data & length */
//		CRC_check()

		/* Copy into struct- (readability) */

	}

	/* Stop collecting data */
	UART_RET = HAL_UART_Transmit(&huart1, MODBUS_WRITE_BUFFER, RTU_MSG_MAXLENGTH, 100);
	if (UART_RET == HAL_OK) {
		HAL_UART_Receive(&huart1, MODBUS_READ_BUFFER, RTU_MSG_MAXLENGTH, 100);
	}


}

void MAX30102_HR_SPO2() {

}

void MAX30102_DIE_TEMP() {

}

static void MODBUS_RTU_format(uint8_t *buffer, uint8_t funct_code) {

	/* Device Address (8-bit) */
	memcpy(buffer, (uint8_t*)MODBUS_DEV_ADDRESS, 1);

	/* Function Code (8-bit) */

	/* Register Address or Valid Bytes (16-bit) */

	/* Register Number or Data (16-bit/N*8-bit) */

	/* CRC check (16-bit) */
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
