/*
 * MPU6050_conf.c
 *
 *
 *  Created on: Nov 15, 2025
 *      Author: noah
 */

#include "MPU6050_conf.h"

// Function implementatino (MPU6050_conf.h)

uint8_t check_memory; // EXTERNAL

void config_I2Cmem(char *buffer, size_t size_buff, uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size){

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, device << 1, memory_add, mem_size, &write_mem, data_size, 100);

	if (status == HAL_OK){
		  snprintf(buffer, size_buff, "Write to register 0x%02X success! (see data-sheet)\n", memory_add);
		  //
		  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
		  //
	  }

	  else {
		  snprintf(buffer, size_buff, "Write to register 0x%02X failed!\n", memory_add);
		  //
		  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
		  //
	  }
	  //Temporary memory-read storage
	  status = HAL_I2C_Mem_Read(&hi2c1, device << 1, memory_add, mem_size, &check_memory, data_size, 100);
	  //Read memory just written-to
	  if (status == HAL_OK){
		  snprintf(buffer, size_buff, "New register 0x%02X value is 0x%02X\n", memory_add, check_memory);
		  //
		  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
		  //
		  }

	  else {
		  snprintf(buffer, size_buff, "Cannot read 0x%02X register...\n", memory_add);
		  //
		  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
		  //
	  }
}

