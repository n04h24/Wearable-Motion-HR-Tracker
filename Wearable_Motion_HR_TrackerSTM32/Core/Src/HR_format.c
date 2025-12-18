/*
 * HR_format.c
 *
 *  Created on: Dec 17, 2025
 *      Author: noah
 */

#include "HR_format.h"

/* Global variables */

void check_device_add() {

	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c2, MAX30102_WRITE, 1, 100);

	if (status == HAL_OK) {
		sprintf(UART, "MAX30102 Writable");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen(UART), 100);
	}

	status = HAL_I2C_IsDeviceReady(&hi2c2, MAX30102_READ, 1, 100);
	if (status == HAL_OK) {
		sprintf(UART, "MAX30102 Readable");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen(UART), 100);
	}
}
