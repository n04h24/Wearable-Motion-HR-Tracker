/*
 * HR_format.c
 *
 *  Created on: Dec 17, 2025
 *      Author: noah
 */

#include "HR_format.h"

/* Global variables */
void MAX30102_config(){

	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c2, MAX30102_ADD << 1, 1, 100);
	if (status == HAL_OK) {
		config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_WR_PTR, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	}
	else {
		snprintf(UART, strlen(UART), "Failed");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen(UART), 100);
	}

}


