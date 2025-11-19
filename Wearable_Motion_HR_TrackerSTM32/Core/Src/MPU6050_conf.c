/*
 * MPU6050_conf.c
 *
 *
 *  Created on: Nov 15, 2025
 *      Author: noah
 */

#include "MPU6050_conf.h"

/* Function implementation (MPU6050_conf.h) */

uint8_t check_memory; // EXTERNAL

void config_I2Cmem(char *buffer, size_t size_buff, uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size){

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, device << 1, memory_add, mem_size, &write_mem, data_size, 100);

	/* Check I2C write = Success */
	if (status == HAL_OK){
		  snprintf(buffer, size_buff, "Write to register 0x%02X success! (see data-sheet)\n", memory_add);
	  }
	  else {
		  snprintf(buffer, size_buff, "Write to register 0x%02X failed!\n", memory_add);

	  }
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

	  /* Validate written memory */
	  status = HAL_I2C_Mem_Read(&hi2c1, device << 1, memory_add, mem_size, &check_memory, data_size, 100);

	  if (status == HAL_OK){
		  snprintf(buffer, size_buff, "New register 0x%02X value is 0x%02X\n", memory_add, check_memory);
		  }

	  else {
		  snprintf(buffer, size_buff, "Cannot read 0x%02X register...\n", memory_add);
	  }
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
}

void calc_FTa(char *buffer, size_t size_buff) {

	/* Retrieval */

	 /* XA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0D, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG13
	 snprintf(buffer, size_buff, "XA_TEST Bits[4-2] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	 uint8_t XA_testA = (check_memory & 0xE0) >> 3;

	 /* YA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0E, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG14
	 snprintf(buffer, size_buff, "YA_TEST Bits[4-2] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	 uint8_t YA_testA = (check_memory & 0xE0) >> 3;

	 /* ZA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0F, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG15
	 snprintf(buffer, size_buff, "ZA_TEST Bits[4-2] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	 uint8_t ZA_testA = (check_memory & 0xE0) >> 3;

	 /* XA_TEST, YA_Test, ZA_TEST (Bit 1-0) */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x10, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG16
	 snprintf(buffer, size_buff, "XA/YA/ZA_TEST Bits[1-0] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

	/* Shift (Bit 1-0) */
	uint8_t XA_testB = (check_memory & 0x30) >> 4;
	uint8_t YA_testB = (check_memory & 0x0C) >> 2;
	uint8_t ZA_testB = (check_memory & 0x03);

	/* Concatenate Bits 4-2 && Bits 2-0 */
	uint8_t XA_TEST = XA_testA | XA_testB;
	uint8_t YA_TEST = YA_testA | YA_testB;
	uint8_t ZA_TEST = ZA_testA | ZA_testB;

	snprintf(buffer, size_buff, "XA_TEST is 0x%02X\nYA_TEST is 0x%02X\nZA_TEST is 0x%02X\n", XA_TEST, YA_TEST, ZA_TEST);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);


	/* Factory Trim Formula */
	float exp(uint8_t TEST_VAL){
		float exponentA = (TEST_VAL-1)/(pow(2,5)-2);
		return exponentA;

	}

	float FT_Xa = 4096 * 0.34 * pow(0.92, exp(XA_TEST))/0.34;
	float FT_Ya = 4096 * 0.34 * pow(0.92, exp(YA_TEST))/0.34;
	float FT_Za = 4096 * 0.34 * pow(0.92, exp(ZA_TEST))/0.34;

	snprintf(buffer, size_buff, "Factory Trims are:\n FT_Xa is %f\nFT_Ya is %f\nFT_Za is %f\n", FT_Xa, FT_Ya, FT_Za);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), 100);

}

void check_accel(char *buffer, size_t size_buff) {



}

