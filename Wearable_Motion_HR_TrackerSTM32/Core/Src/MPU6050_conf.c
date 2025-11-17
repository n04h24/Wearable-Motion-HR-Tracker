/*
 * MPU6050_conf.c
 *
 *
 *  Created on: Nov 15, 2025
 *      Author: noah
 */

#include "MPU6050_conf.h"

// Function implementation (MPU6050_conf.h)

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

void self_testXYZ(char *buffer, size_t size_buff) {

	//TEST X(1)
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0D, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		  //Read Self-Test X
	 snprintf(buffer, size_buff, "Self-Test X-value is 0x%02X\n", check_memory);
		  //Format Self-Test X (string)
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);
		  //Print to UART
		  //Value is: 0b1101110
	 uint8_t XA_testA = (check_memory & 0xE0) >> 3;

	 //TEST Y(1)
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0E, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		  //Read Self-Test Y
	 snprintf(buffer, size_buff, "Self-Test Y-value is 0x%02X\n", check_memory);
	 	  //Format Self-Test Y (string)
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);
	 	  //Print to UART
	 	  //Value is: 0b01101111
	 uint8_t YA_testA = (check_memory & 0xE0) >> 3;

	 //TEST Z(1)
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0F, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		  //Read Self-Test Z
	 snprintf(buffer, size_buff, "Self-Test Z-value is 0x%02X\n", check_memory);
	 	  //Format Self-Test Z (string)
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);
	 	  //Print to UART
	 	  //Value is: 0b10010101
	 uint8_t ZA_testA = (check_memory & 0xE0) >> 3;

	 //TEST XYZ(2)
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x10, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		  //Read Self-Test X
		snprintf(buffer, size_buff, "Self-Test XYZ-value is 0x%02X\n", check_memory);
			  //Format Self-Test X (string)
		HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);
			  //Print to UART
			  //Value is: 0b01100111

	uint8_t XA_testB = (check_memory & 0x30) >> 4;
	uint8_t YA_testB = (check_memory & 0x0C) >> 2;
	uint8_t ZA_testB = (check_memory & 0x03);

	uint8_t XA_test = XA_testA | XA_testB; //
	snprintf(buffer, size_buff, "Concatenated X Self-Test is 0x%02X\n", XA_test);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	uint8_t YA_test = YA_testA | YA_testB;
	snprintf(buffer, size_buff, "Concatenated Y Self-Test is 0x%02X\n", YA_test);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	uint8_t ZA_test = ZA_testA | ZA_testB;
	snprintf(buffer, size_buff, "Concatenated Z Self-Test is 0x%02X\n", ZA_test);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	   /*Therefore, concatenated:b
	    * X:(000)01110
	    * Y:(000)01101
	    * Z:(000)10011
	    *
		*/
}

void factory_trim() {
	//Null
}

