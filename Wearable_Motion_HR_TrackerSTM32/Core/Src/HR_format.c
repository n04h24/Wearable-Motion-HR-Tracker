/*
 * HR_format.c
 *
 *  Created on: Dec 17, 2025
 *      Author: noah
 */

#include "HR_format.h"

/* Global variables */

void MAX30102_init() {
	/* Bring FIFO to known state: 0x00 */
	config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_WR_PTR, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	config_I2Cmem(&hi2c2, MAX30102_ADD, OVF_COUNTER, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_RD_PTR, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	/* Sample averaging (0) and FLAG (18) unread samples */
	config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_CONFIG, 0x0E, I2C_MEMADD_SIZE_8BIT, 1);
	/* Open HR (& Red ONLY) mode (010) */
	config_I2Cmem(&hi2c2, MAX30102_ADD, MODE_CONFIG, 0x02, I2C_MEMADD_SIZE_8BIT, 1);
	/* Set pulse-width to _|‾‾|__|‾‾|__ 411ns (MAX) & 18-bit (MAX resolution) ADC */
	config_I2Cmem(&hi2c2, MAX30102_ADD, SPO2_CONFIG, 0x03, I2C_MEMADD_SIZE_8BIT, 1);
	/* Set (red) LED current to 51mA (MAX) */
	config_I2Cmem(&hi2c2, MAX30102_ADD, LED1_PA, 0x0C, I2C_MEMADD_SIZE_8BIT, 1);
}

void MAX30102_temp() {
	/* Initialise (temporary) storage */
	int8_t temp_integer;
	int8_t temp_fraction;
	/* Enable temperature reading (self-clearing bit)*/
	config_I2Cmem(&hi2c2, MAX30102_ADD, TEMP_EN, 0x01, I2C_MEMADD_SIZE_8BIT, 1);
	/* Check INT ready */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, DIE_TEMP_READY, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	/* Read & store integer */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, TEMP_INT, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	temp_integer = (int8_t*)check_memory;
	/* Read & store fraction */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, TEMP_FRAC, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	temp_fraction = (int8_t*)check_memory;
	/* NB: Casting type-pointer reinterprets memory itself */
	int hold = 0;
}
