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

void config_I2Cmem(uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size){

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, device << 1, memory_add, mem_size, &write_mem, data_size, 100);

	/* Check I2C write = Success */
	if (status == HAL_OK){
		  snprintf(UART, UART_BUFF_SIZE, "Write to register 0x%02X success! (see data-sheet)\n", memory_add);
	  }
	  else {
		  snprintf(UART, UART_BUFF_SIZE, "Write to register 0x%02X failed!\n", memory_add);

	  }
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	  /* Validate written memory */
	  status = HAL_I2C_Mem_Read(&hi2c1, device << 1, memory_add, mem_size, &check_memory, data_size, 100);

	  if (status == HAL_OK){
		  snprintf(UART, UART_BUFF_SIZE, "New register 0x%02X value is 0x%02X\n", memory_add, check_memory);
		  }

	  else {
		  snprintf(UART, UART_BUFF_SIZE, "Cannot read 0x%02X register...\n", memory_add);
	  }
	 HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
}

void calc_FTa(char *buffer, size_t size_buff) {

	/* Retrieval */

	 /* XA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0D, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG13
	 snprintf(UART, UART_BUFF_SIZE, "XA_TEST Bits[4-2] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	 uint8_t XA_testA = (check_memory & 0xE0) >> 3;

	 /* YA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0E, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG14
	 snprintf(UART, UART_BUFF_SIZE, "YA_TEST Bits[4-2] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	 uint8_t YA_testA = (check_memory & 0xE0) >> 3;

	 /* ZA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0F, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG15
	 snprintf(UART, UART_BUFF_SIZE, "ZA_TEST Bits[4-2] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	 uint8_t ZA_testA = (check_memory & 0xE0) >> 3;

	 /* XA_TEST, YA_Test, ZA_TEST (Bit 1-0) */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x10, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG16
	 snprintf(UART, UART_BUFF_SIZE, "XA/YA/ZA_TEST Bits[1-0] are encoded in 0x%02X\n", check_memory);
	 HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift (Bit 1-0) */
	uint8_t XA_testB = (check_memory & 0x30) >> 4;
	uint8_t YA_testB = (check_memory & 0x0C) >> 2;
	uint8_t ZA_testB = (check_memory & 0x03);

	/* Concatenate Bits 4-2 && Bits 2-0 */
	uint8_t XA_TEST = XA_testA | XA_testB;
	uint8_t YA_TEST = YA_testA | YA_testB;
	uint8_t ZA_TEST = ZA_testA | ZA_testB;

	snprintf(UART, UART_BUFF_SIZE, "XA_TEST is 0x%02X\nYA_TEST is 0x%02X\nZA_TEST is 0x%02X\n", XA_TEST, YA_TEST, ZA_TEST);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);


	/* Factory Trim Formula */
	float exp(uint8_t TEST_VAL){
		float exponentA = (TEST_VAL-1)/(pow(2,5)-2);
		return exponentA;

	}

	float FT_Xa = 4096 * 0.34 * pow(0.92, exp(XA_TEST))/0.34;
	float FT_Ya = 4096 * 0.34 * pow(0.92, exp(YA_TEST))/0.34;
	float FT_Za = 4096 * 0.34 * pow(0.92, exp(ZA_TEST))/0.34;

	snprintf(UART, UART_BUFF_SIZE, "Factory Trims are:\n FT_Xa is %f\nFT_Ya is %f\nFT_Za is %f\n", FT_Xa, FT_Ya, FT_Za);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

}

void check_accel() {

	/* X acceleration */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_X_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (High) Value is 0x%02X\n", ACCEL_X_H);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_X_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (Low) Value is 0x%02X\n", ACCEL_X_L);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	uint16_t ACCEL_X = (ACCEL_X_H << 8) | ACCEL_X_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw X Value is: %" PRId16 "\n", ACCEL_X);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	ACCEL_X = ACCEL_X / 4096;
	snprintf(UART, UART_BUFF_SIZE, "X Value divided by sensitivity is: %" PRId16 "\n", ACCEL_X);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);


}

