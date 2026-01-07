/*
 * HR_format.c
 *
 *  Created on: Dec 17, 2025
 *      Author: noah
 */

#include "HR_format.h"

/* Global variables */
uint8_t die_sample = 0;
/* FIFO Buffer */
float MAX30102_DieTemp[20];

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
	/* Enable Interrupts: FIFO_Almost_Full & FIFO_Data_Ready (1); DIE_TEMP_Ready (2) */
//	config_I2Cmem(&hi2c2, MAX30102_ADD, INT_EN1, 0xC0, I2C_MEMADD_SIZE_8BIT, 1);
	config_I2Cmem(&hi2c2, MAX30102_ADD, INT_EN2, 0x02, I2C_MEMADD_SIZE_8BIT, 1);
	/* Check PWR_RDY (to take measurements) */
	while (!(check_memory & 0x01)) {
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT1, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	}
	/* Check state of DIE_TEMP_RDY */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT2, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	/* ERROR (Result): Binary:1011000 */
	if (!(check_memory & 0x02)) {
		/* Print statement */
	}
}

void MAX30102_temp() {
	/* Initialise (temporary) storage */
	int8_t TEMP_INT_VAL = 0;
	float TEMP_FRAC_VAL = 0;
	/* Enable temperature reading (self-clearing bit)*/
	config_I2Cmem(&hi2c2, MAX30102_ADD, TEMP_EN, 0x01, I2C_MEMADD_SIZE_8BIT, 1);
	/* Check INT ready (complete conversion */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT2, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);

//	if (check_memory == 0x02) {
		/* Read & store integer (unsigned-int) */
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, TEMP_INT, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		TEMP_INT_VAL = (int8_t)check_memory;
		/* Read & store fraction (float; 32-bit) */
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, TEMP_FRAC, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		TEMP_FRAC_VAL = 0.0625 * (float)check_memory;
			/* NB: Casting type-pointer reinterprets memory itself */

		/* Store die temperature (float) */
		MAX30102_DieTemp[die_sample] = (float)TEMP_INT_VAL + TEMP_FRAC_VAL;
		die_sample++;
		/* Reset DIE_TEMP_RDY (INT2) */
		config_I2Cmem(&hi2c2, MAX30102_ADD, INT_STAT2, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
//	}
}

void MAX30102_HR() {

	/* Check PPG (INT1) */

	/* Check unread samples (pointer-wrap) */
		// WR_pointer
		// RD_pointer

	/* Retrieve & append 3-Bytes for NUM_SAMPLES_READ */

	/* Incremental check for RD & WR */
		// WR_pointer
		// RD_pointer

}
