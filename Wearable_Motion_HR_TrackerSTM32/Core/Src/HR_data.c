/*
 * HR_data.c
 *
 *  Created on: Jan 19, 2026
 *      Author: noah
 */

/* Includes */
#include "HR_data.h"

/* Variable definitions */
uint8_t MODBUS_WRITE_MSG[RTU_MSG_MAXLENGTH];
uint8_t MODBUS_READ_MSG[RTU_MSG_MAXLENGTH];

/* Implementations */
void MAX30102_collect() {
	/* Check response */
	HAL_StatusTypeDef UART_RET = HAL_UART_Transmit(&huart1, MODBUS_WRITE_MSG, RTU_MSG_MAXLENGTH, 100);
	if (UART_RET == HAL_OK) {
		HAL_UART_Receive(&huart1, MODBUS_READ_MSG, RTU_MSG_MAXLENGTH, 100);
	}
}

void MAX30102_HR_SPO2() {

}

void MAX30102_DIE_TEMP() {

}
