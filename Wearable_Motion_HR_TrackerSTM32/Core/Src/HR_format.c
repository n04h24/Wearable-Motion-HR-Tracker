/*
 * HR_format.c
 *
 *  Created on: Dec 17, 2025
 *      Author: noah
 */

#include "HR_format.h"

/* Global variables */
uint8_t die_sample = 0;
uint8_t check_memoryMAX = 0;
/* FIFO Buffer */
float MAX30102_DieTemp[20];

void MAX30102_init() {
	/* Clear "latched" interrupts */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT1, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT2, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
	/* Soft Reset */
	config_I2Cmem(&hi2c2, MAX30102_ADD, MODE_CONFIG, 0x40, I2C_MEMADD_SIZE_8BIT, 1);
	HAL_Delay(10);
	/* Bring FIFO to known state: 0x00 */
	config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_WR_PTR, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	config_I2Cmem(&hi2c2, MAX30102_ADD, OVF_COUNTER, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_RD_PTR, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	/* Sample averaging (4) and FLAG (17) unread samples (a.k.a 15-empty slots). No Rollover */
	config_I2Cmem(&hi2c2, MAX30102_ADD, FIFO_CONFIG, 0x4F, I2C_MEMADD_SIZE_8BIT, 1);
	/* Set pulse-width to _|‾‾|__|‾‾|__ 411microsec (MAX) & 18-bit ADC Resolution */
	/* 8192 nA ADC Range Control & 100 sps */
	config_I2Cmem(&hi2c2, MAX30102_ADD, SPO2_CONFIG, 0x87, I2C_MEMADD_SIZE_8BIT, 1);
	/* Set (red) LED current to 51mA (MAX) */
	config_I2Cmem(&hi2c2, MAX30102_ADD, LED1_PA, 0x24, I2C_MEMADD_SIZE_8BIT, 1);
	/* Set (IR) LED current to OFF */
	config_I2Cmem(&hi2c2, MAX30102_ADD, LED2_PA, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	/* Enable Interrupts: FIFO_Almost_Full & FIFO_Data_Ready (1); DIE_TEMP_Ready (2) */
	config_I2Cmem(&hi2c2, MAX30102_ADD, INT_EN1, 0xC0, I2C_MEMADD_SIZE_8BIT, 1);
	config_I2Cmem(&hi2c2, MAX30102_ADD, INT_EN2, 0x02, I2C_MEMADD_SIZE_8BIT, 1);
	/* HR mode enable */
	config_I2Cmem(&hi2c2, MAX30102_ADD, MODE_CONFIG, 0x02, I2C_MEMADD_SIZE_8BIT, 1);
	/* Clear "latched" interrupts */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT1, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
}

void MAX30102_temp() {
	/* Initialise (temporary) storage */
	int8_t TEMP_INT_VAL = 0;
	float TEMP_FRAC_VAL = 0;
	/* Enable temperature reading (self-clearing bit)*/
	config_I2Cmem(&hi2c2, MAX30102_ADD, TEMP_EN, 0x01, I2C_MEMADD_SIZE_8BIT, 1);
	HAL_Delay(10);
	/* Confirm conversion (INT) */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT2, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);

	if (check_memoryMAX == 0x02) {
		/* Read & store integer (unsigned-int) */
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, TEMP_INT, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
		TEMP_INT_VAL = (int8_t)check_memoryMAX;
		/* Read & store fraction (float; 32-bit) */
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, TEMP_FRAC, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
		TEMP_FRAC_VAL = 0.0625 * (float)check_memoryMAX;
			/* NB: Casting type-pointer reinterprets memory itself */

		/* Store die temperature (float) */
		MAX30102_DieTemp[die_sample] = (float)TEMP_INT_VAL + TEMP_FRAC_VAL;
		die_sample++;
		/* Reset DIE_TEMP_RDY (INT2) */
//		config_I2Cmem(&hi2c2, MAX30102_ADD, INT_STAT2, 0x00, I2C_MEMADD_SIZE_8BIT, 1);
	}
}

void MAX30102_HR() {

	/* Check PPG (INT1) */
	HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, INT_STAT1, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
	/* Check unread samples (pointer-wrap) */
		// WR_pointer
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, FIFO_WR_PTR, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);
		// RD_pointer
		HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADD << 1, FIFO_RD_PTR, I2C_MEMADD_SIZE_8BIT, &check_memoryMAX, 1, 100);

	/* Retrieve & append 3-Bytes for NUM_SAMPLES_READ */

	/* Incremental check for RD & WR */
		// WR_pointer
		// RD_pointer

}

/* ––––––––––––––––––––––––––––––––––––––– TEST FUNCTION */

volatile HAL_StatusTypeDef g_ready_st_7  = HAL_ERROR;
volatile HAL_StatusTypeDef g_ready_st_8  = HAL_ERROR;
volatile uint32_t          g_ready_err_7 = 0;
volatile uint32_t          g_ready_err_8 = 0;

volatile HAL_StatusTypeDef g_read_st_7   = HAL_ERROR;
volatile HAL_StatusTypeDef g_read_st_8   = HAL_ERROR;
volatile uint32_t          g_read_err_7  = 0;
volatile uint32_t          g_read_err_8  = 0;

volatile uint8_t g_part_7 = 0x00, g_rev_7 = 0x00, g_int2_7 = 0x00;
volatile uint8_t g_part_8 = 0x00, g_rev_8 = 0x00, g_int2_8 = 0x00;



void I2C2_MAX30102_TestFunction() {

  // ---- Test A: correct STM32 HAL convention (pass 8-bit address byte = 7bit<<1) ----
	uint16_t addr_hal = (MAX30102_ADDR_7BIT << 1);     // 0xAE

	g_ready_st_7  = HAL_I2C_IsDeviceReady(&hi2c2, addr_hal, 3, 20);
	g_ready_err_7 = HAL_I2C_GetError(&hi2c2);

	g_read_st_7   = read_reg(addr_hal, REG_PART_ID, (uint8_t*)&g_part_7);
	g_read_err_7  = HAL_I2C_GetError(&hi2c2);

	(void)read_reg(addr_hal, REG_REV_ID,     (uint8_t*)&g_rev_7);
	(void)read_reg(addr_hal, REG_INT_STATUS2,(uint8_t*)&g_int2_7);

	// ---- Test B: WRONG-on-purpose (treat 0x57 as if it were already HAL addr) ----
	// This helps detect “double shift” / mixed conventions elsewhere.
	uint16_t addr_wrong = (MAX30102_ADDR_7BIT);       // 0x57 (not shifted)

	g_ready_st_8  = HAL_I2C_IsDeviceReady(&hi2c2, addr_wrong, 3, 20);
	g_ready_err_8 = HAL_I2C_GetError(&hi2c2);

	g_read_st_8   = read_reg(addr_wrong, REG_PART_ID, (uint8_t*)&g_part_8);
	g_read_err_8  = HAL_I2C_GetError(&hi2c2);

	(void)read_reg(addr_wrong, REG_REV_ID,      (uint8_t*)&g_rev_8);
	(void)read_reg(addr_wrong, REG_INT_STATUS2, (uint8_t*)&g_int2_8);

	// Put a breakpoint here and inspect:
	// g_ready_st_7, g_part_7, g_rev_7, g_int2_7, g_read_err_7
	// g_ready_st_8, g_part_8, ...

}

void I2C2_MAX30102_FailureMode() {

	uint16_t addr = (0x57 << 1); // 0xAE
	HAL_StatusTypeDef st_ready = HAL_I2C_IsDeviceReady(&hi2c2, addr, 3, 20);
	uint32_t err_ready = HAL_I2C_GetError(&hi2c2);
	// breakpoint: st_ready, err_ready
	uint8_t part_mem = 0;
	HAL_StatusTypeDef st_mem = HAL_I2C_Mem_Read(&hi2c2, addr, 0xFF,
	                                           I2C_MEMADD_SIZE_8BIT,
	                                           &part_mem, 1, 100);
	uint32_t err_mem = HAL_I2C_GetError(&hi2c2);
	// breakpoint: st_mem, err_mem, part_mem
	uint8_t reg = 0xFF;
	uint8_t part_seq = 0;

	HAL_StatusTypeDef st_tx = HAL_I2C_Master_Transmit(&hi2c2, addr, &reg, 1, 100);
	uint32_t err_tx = HAL_I2C_GetError(&hi2c2);

	HAL_StatusTypeDef st_rx = HAL_I2C_Master_Receive(&hi2c2, addr, &part_seq, 1, 100);
	uint32_t err_rx = HAL_I2C_GetError(&hi2c2);
	// breakpoint: st_tx, err_tx, st_rx, err_rx, part_seq
}

void I2C2_MAX30102_ACKing() {

	volatile uint8_t found[128] = {0};
		/* ACKs for '87' (0x57 – MAX30102) */
	    for (uint8_t a = 1; a < 127; a++) {
	        if (HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(a << 1), 2, 10) == HAL_OK) {
	            found[a] = 1;
	    }
	}
}

void I2C2_MAX30102_ReadRegisters() {

	    uint16_t addr = (0x57 << 1);
	    /* Testing (known) values of addresses */
	    volatile uint8_t part = rd(&hi2c2, addr, 0xFF);
	    volatile uint8_t rev  = rd(&hi2c2, addr, 0xFE);
	    volatile uint8_t mode = rd(&hi2c2, addr, 0x09);
	    volatile uint8_t int1 = rd(&hi2c2, addr, 0x00);
	    volatile uint8_t int2 = rd(&hi2c2, addr, 0x01);

}
