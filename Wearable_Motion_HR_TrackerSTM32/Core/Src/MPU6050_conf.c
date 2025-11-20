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

FactoryTrims FT; // EXTERNAL

Offsets acceleration;

uint8_t ACCEL_X_H;
uint8_t ACCEL_X_L;
uint8_t ACCEL_Y_H;
uint8_t ACCEL_Y_L;
uint8_t ACCEL_Z_H;
uint8_t ACCEL_Z_L;

/* Implementations */

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

	FT.Xa = 4096 * 0.34 * pow(0.92, exp(XA_TEST))/0.34;
	FT.Ya = 4096 * 0.34 * pow(0.92, exp(YA_TEST))/0.34;
	FT.Za = 4096 * 0.34 * pow(0.92, exp(ZA_TEST))/0.34;

	snprintf(UART, UART_BUFF_SIZE, "Factory Trims are:\n FT_Xa is %f\nFT_Ya is %f\nFT_Za is %f\n", FT.Xa, FT.Ya, FT.Za);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

}

void check_accel() {

	/* Retrieve upper-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_X_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (High) Value is 0x%02X\n", ACCEL_X_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_X_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (Low) Value is 0x%02X\n", ACCEL_X_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	uint16_t rawACCEL_X = (ACCEL_X_H << 8) | ACCEL_X_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw X Value is: %" PRId16 "\n", rawACCEL_X);
	//Signed 16-bit specifier
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	//Prints +/-

	/* Divide by LSB/g value */
	float ACCEL_X = (float) rawACCEL_X / 4096;
	snprintf(UART, UART_BUFF_SIZE, "X Value divided by sensitivity is: %fs^2\n", ACCEL_X);
	//ERROR:
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);


	/* Retrieve upper-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_Y_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Y (High) Value is 0x%02X\n", ACCEL_Y_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_Y_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Y (Low) Value is 0x%02X\n", ACCEL_Y_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	uint16_t rawACCEL_Y = (ACCEL_Y_H << 8) | ACCEL_Y_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw Y Value is: %" PRId16 "\n", rawACCEL_Y);
	//Signed 16-bit specifier
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	//Prints +/-

	/* Divide by LSB/g value */
	float ACCEL_Y = (float) rawACCEL_Y / 4096;
	snprintf(UART, UART_BUFF_SIZE, "Y Value divided by sensitivity is: %fs^2\n", ACCEL_Y);
	//ERROR:
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);


	/* Retrieve upper-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_Z_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Z (High) Value is 0x%02X\n", ACCEL_Z_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	uint8_t ACCEL_Z_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Z (Low) Value is 0x%02X\n", ACCEL_Z_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	uint16_t rawACCEL_Z = (ACCEL_Z_H << 8) | ACCEL_Z_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw Z Value is: %" PRId16 "\n", rawACCEL_Z);
	//Signed 16-bit specifier
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	//Prints +/-

	/* Divide by LSB/g value */
	float ACCEL_Z = (float) rawACCEL_Z / 4096;
	snprintf(UART, UART_BUFF_SIZE, "Z Value divided by sensitivity is: %fs^2\n", ACCEL_Z);
	//ERROR:
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

}

void startSFT() {

	/* Retrieve upper-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_X_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (High) Value is 0x%02X\n", ACCEL_X_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_X_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (Low) Value is 0x%02X\n", ACCEL_X_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	int16_t rawACCEL_X1 = (ACCEL_X_H << 8) | ACCEL_X_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw X Value is: %" PRId16 "\n", rawACCEL_X1);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve upper-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Y_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Y (High) Value is 0x%02X\n", ACCEL_Y_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Y_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Y (Low) Value is 0x%02X\n", ACCEL_Y_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	int16_t rawACCEL_Y1 = (ACCEL_Y_H << 8) | ACCEL_Y_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw Y Value is: %" PRId16 "\n", rawACCEL_Y1);
	//Signed 16-bit specifier
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve upper-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Z_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Z (High) Value is 0x%02X\n", ACCEL_Z_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Z_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Z (Low) Value is 0x%02X\n", ACCEL_Z_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	int16_t rawACCEL_Z1 = (ACCEL_Z_H << 8) | ACCEL_Z_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw Z Value is: %" PRId16 "\n", rawACCEL_Z1);
	//Signed 16-bit specifier
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	//Prints +/-

	/* Before Self-Test */
	config_I2Cmem(MPU6050, ACCEL_CONFIG, 0xF0, I2C_MEMADD_SIZE_8BIT, 1);
	/* After Self-Test */
	/* Triggers accelerometer self-test; ±8g for XYZ */

	/* Retrieve upper-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_X_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (High) Value is 0x%02X\n", ACCEL_X_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_X_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "X (Low) Value is 0x%02X\n", ACCEL_X_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	int16_t rawACCEL_X2 = (ACCEL_X_H << 8) | ACCEL_X_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw X Value is: %" PRId16 "\n", rawACCEL_X2);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve upper-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Y_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Y (High) Value is 0x%02X\n", ACCEL_Y_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Y_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Y (Low) Value is 0x%02X\n", ACCEL_Y_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	int16_t rawACCEL_Y2 = (ACCEL_Y_H << 8) | ACCEL_Y_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw Y Value is: %" PRId16 "\n", rawACCEL_Y2);
	//Signed 16-bit specifier
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve upper-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Z_H = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Z (High) Value is 0x%02X\n", ACCEL_Z_H);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Retrieve lower-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Z_L = check_memory;
	snprintf(UART, UART_BUFF_SIZE, "Z (Low) Value is 0x%02X\n", ACCEL_Z_L);
//	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

	/* Shift upper << 8; | operation to combine lower */
	int16_t rawACCEL_Z2 = (ACCEL_Z_H << 8) | ACCEL_Z_L;
	snprintf(UART, UART_BUFF_SIZE, "Raw Z Value is: %" PRId16 "\n", rawACCEL_Z2);
	//Signed 16-bit specifier
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	//Prints +/-

	/* Change% Equation */
	float X_SFT = ((rawACCEL_X2 - rawACCEL_X1) - FT.Xa)/FT.Xa + 1;
	float Y_SFT = ((rawACCEL_Y2 - rawACCEL_Y1) - FT.Ya)/FT.Ya + 1;
	float Z_SFT = ((rawACCEL_Z2 - rawACCEL_Z1) - FT.Za)/FT.Za + 1;
	//(SML value – LRG Trim) / LRG Trim = Negative

	/* PASS/FAIL condition */
	if ((X_SFT < 0.15) && (X_SFT > -0.15)){
		snprintf(UART, UART_BUFF_SIZE, "X = PASS\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	}
	if ((Y_SFT < 0.15) && (Y_SFT > -0.15)){
		snprintf(UART, UART_BUFF_SIZE, "Y = PASS\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	}
	if ((Z_SFT < 0.15) && (Z_SFT > -0.15)){
		snprintf(UART, UART_BUFF_SIZE, "Z = PASS\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);
	}


	snprintf(UART, UART_BUFF_SIZE, "X Self-Test = %f\nY Self-Test = %f\nZ Self-Test = %f\n", X_SFT, Y_SFT, Z_SFT);
	HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);


}

void accel_calibration(Offsets *acceleration_offs) {

	float sum_ACCEL_X = 0;
	float sum_ACCEL_Y = 0;
	float sum_ACCEL_Z = 0;

	for (int i = 0; i <= 100; i++){

		/* Retrieve upper-X value */
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		ACCEL_X_H = check_memory;

		/* Retrieve lower-X value */
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		ACCEL_X_L = check_memory;

		/* Shift upper << 8; | operation to combine lower */
		int16_t sample_ACCEL_X = (ACCEL_X_H << 8) | ACCEL_X_L;

		/* Retrieve upper-X value */
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		ACCEL_Y_H = check_memory;

		/* Retrieve lower-X value */
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		ACCEL_Y_L = check_memory;

		/* Shift upper << 8; | operation to combine lower */
		int16_t sample_ACCEL_Y = (ACCEL_Y_H << 8) | ACCEL_Y_L;

		/* Retrieve upper-X value */
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		ACCEL_Z_H = check_memory;

		/* Retrieve lower-X value */
		HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
		ACCEL_Z_L = check_memory;

		/* Shift upper << 8; | operation to combine lower */
		int16_t sample_ACCEL_Z = (ACCEL_Z_H << 8) | ACCEL_Z_L;

		/* Convert acceleration to m/s^2 */
		float conv_ACCEL_X = (float) sample_ACCEL_X / 4096;
		float conv_ACCEL_Y= (float) sample_ACCEL_Y / 4096;
		float conv_ACCEL_Z = (float) sample_ACCEL_Z / 4096;

		/* Sum up to 100 samples */
		sum_ACCEL_X =+ conv_ACCEL_X;

		/* Confirm sampling summation */
		snprintf(UART, UART_BUFF_SIZE, "X_sum = %f\n",sum_ACCEL_X);
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen((char*)UART), 100);

		sum_ACCEL_Y =+ conv_ACCEL_Y;
		sum_ACCEL_Z =+ conv_ACCEL_Z;

	}

	/* Calculate offset */
	acceleration_offs->x = sum_ACCEL_X / 100;
	acceleration_offs->y = sum_ACCEL_Y / 100;
	acceleration_offs->z = sum_ACCEL_Z / 100;

	//Retrieve ~100 samples

}
