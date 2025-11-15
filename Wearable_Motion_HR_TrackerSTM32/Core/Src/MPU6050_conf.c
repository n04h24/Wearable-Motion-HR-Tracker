/*
 * MPU6050_conf.c
 *
 *
 *  Created on: Nov 15, 2025
 *      Author: noah
 */

#include "MPU6050_conf.h"

// Function implementatino (MPU6050_conf.h)

uint8_t check_mem;

void config_I2Cmem(uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size){

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, device << 1, memory_add, mem_size, &write_mem, data_size, 100);

	if (status == HAL_OK){
		  snprintf(UART, sizeof(UART), "Write to register 0x%02X success! (see datasheet)\n", memory_add);
//		  //
//		  HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
//		  //
	  }

	  else {
//		  snprintf(UART, sizeof(UART), "Write to register 0x%02X failed!\n", memory_add);
//		  //
//		  HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
//		  //
	  }
}


