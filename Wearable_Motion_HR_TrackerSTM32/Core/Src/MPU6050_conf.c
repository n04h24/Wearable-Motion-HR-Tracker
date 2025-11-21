/*
 * MPU6050_conf.c
 *
 *
 *  Created on: Nov 15, 2025
 *      Author: noah
 */

/* Includes */
#include "MPU6050_conf.h"

/* External variables */
uint8_t check_memory;

Output FT;
Output Offsets;

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
	if (status != HAL_OK){
	  //Error message*
	}
}

void calculate_FACT() {

	/* Retrieval */

	 /* XA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0D, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG13
	 uint8_t XA_testA = (check_memory & 0xE0) >> 3;

	 /* YA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0E, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG14
	 uint8_t YA_testA = (check_memory & 0xE0) >> 3;

	 /* ZA_TEST (Bit 4-2)  */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x0F, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG15
	 uint8_t ZA_testA = (check_memory & 0xE0) >> 3;

	 /* XA_TEST, YA_Test, ZA_TEST (Bit 1-0) */
	 HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, 0x10, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	 // Read from REG16

	/* Shift (Bit 1-0) */
	uint8_t XA_testB = (check_memory & 0x30) >> 4;
	uint8_t YA_testB = (check_memory & 0x0C) >> 2;
	uint8_t ZA_testB = (check_memory & 0x03);

	/* Concatenate Bits 4-2 && Bits 2-0 */
	uint8_t XA_TEST = XA_testA | XA_testB;
	uint8_t YA_TEST = YA_testA | YA_testB;
	uint8_t ZA_TEST = ZA_testA | ZA_testB;

	/* Factory Trim Formula */
	float exp(uint8_t TEST_VAL){
		float exponentA = (TEST_VAL-1)/(pow(2,5)-2);
		return exponentA;
	}

	/* Equate in FactoryTrims struct */
	FT.X = 4096 * 0.34 * pow(0.92, exp(XA_TEST))/0.34;
	FT.Y = 4096 * 0.34 * pow(0.92, exp(YA_TEST))/0.34;
	FT.Z = 4096 * 0.34 * pow(0.92, exp(ZA_TEST))/0.34;


void test_RESPONSE() {

	/* Before Self-Test */
	int16_t raw_X1 = 0;
	int16_t raw_Y1 = 0;
	int16_t raw_Z1 = 0;

	/* Read + append (2) 8-bit ACCEL_OUT registers */
	readA_CONCAT(&raw_X1, &raw_Y1, &raw_Z1);

	/* Triggers SFT; ±8g for XYZ */
	config_I2Cmem(MPU6050, ACCEL_CONFIG, 0xF0, I2C_MEMADD_SIZE_8BIT, 1);

	/* After Self-Test */
	int16_t raw_X2 = 0;
	int16_t raw_Y2 = 0;
	int16_t raw_Z2 = 0;

	/* Read + append (2) 8-bit ACCEL_OUT registers */
	readA_CONCAT(&raw_X2, &raw_Y2, &raw_Z2);

	/* Change% Equation */
	float X_SFT = ((raw_X2 - raw_X1) - FT.X)/FT.X + 1;
	float Y_SFT = ((raw_Y2 - raw_Y1) - FT.Y)/FT.Y + 1;
	float Z_SFT = ((raw_Z2 - raw_Z1) - FT.Z)/FT.Z + 1;
	//(+1) adjusts negative value / reveals percentage (%)

	/* Fail output */
	if ((X_SFT > 0.15) && (X_SFT < -0.15)){
		//Error X message
	}
	if ((Y_SFT > 0.15) && (Y_SFT < -0.15)){
		//Error Y message
	}
	if ((Z_SFT > 0.15) && (Z_SFT < -0.15)){
		//Error Z message
	}

}

void readA_CONCAT(int16_t *raw_X, int16_t *raw_Y, int16_t *raw_Z) {

	/* Retrieve upper-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_X_H = check_memory;

	/* Retrieve lower-X value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_X_L = check_memory;

	/* Shift upper << 8; | operation to combine lower */
	*raw_X = (ACCEL_X_H << 8) | ACCEL_X_L;

	/* Retrieve upper-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Y_H = check_memory;

	/* Retrieve lower-Y value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Y_L = check_memory;

	/* Shift upper << 8; | operation to combine lower */
	*raw_Y = (ACCEL_Y_H << 8) | ACCEL_Y_L;

	/* Retrieve upper-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Z_H = check_memory;

	/* Retrieve lower-Z value */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &check_memory, 1, 100);
	ACCEL_Z_L = check_memory;

	/* Shift upper << 8; | operation to combine lower */
	*raw_Z = (ACCEL_Z_H << 8) | ACCEL_Z_L;

}

void calculate_OFFS() {

	/* Initialise sum of XYZ */
	float sum_X = 0;
	float sum_Y = 0;
	float sum_Z = 0;

	/* Sample n times */
	uint8_t n = 100;

	/* Retrieve & sum 'n' samples */
	for (int i = 0; i <= n; i++){

		/* Temporary sample storage */
		int16_t sample_X = 0;
		int16_t sample_Y = 0;
		int16_t sample_Z = 0;

		readA_CONCAT(&sample_X, &sample_Y, &sample_Z);

		/* Convert acceleration to m/s^2 */
		float conv_X = (float) sample_X / 4096;
		float conv_Y= (float) sample_Y / 4096;
		float conv_Z = (float) sample_Z / 4096;

		/* Sum samples */
		sum_X =+ conv_X;
		sum_Y =+ conv_Y;
		sum_Z =+ conv_Z;
	}

	/* Calculate offset */
	Offsets.X = sum_X / n;
	Offsets.Y = sum_Y / n;
	Offsets.Z = sum_Z / n;

}

void calibrate_ACCEL() {

	/*1.  (Interrupt/Poll) –> Call function
	2.	Read/Store Upper & Lower: set (expand)
	3.	Concatenate: set
	4.	Convert (float): set

	5.	Apply offset
	6.	Return */

	/* Temporary 16-bit storage (raw) */
	int16_t raw_X = 0;
	int16_t raw_Y = 0;
	int16_t raw_Z = 0;

	/* Retrieval and concatenation (int16_t) */
	readA_CONCAT(&raw_X, &raw_Y, &raw_Z);

	/* Conversion (float) */
	float conversionX = (float) raw_X / 4096;
	float conversionY= (float) raw_Y / 4096;
	float conversionZ = (float) raw_Z / 4096;

	/* Calibration (Offset) */
	Acceleration.X = conversionX - Offsets.X;
	Acceleration.Y = conversionY - Offsets.Y;
	Acceleration.Z = conversionZ - Offsets.Z;

}

void MPU6050_init() {

	/* Wake device */
	config_I2Cmem(MPU6050, PWR_MGMT_1, 0x00, I2C_MEMADD_SIZE_8BIT, 1);

	/* Retrieve factory trims */
	calculate_FACT();

	/* Determine pass/fail */
	test_RESPONSE();

	/* Sample to 79; 100x a second */
	config_I2Cmem(MPU6050, SMPLRT_DIV, 0x4F, I2C_MEMADD_SIZE_8BIT, 1);

	/* Output "starting" calibrated acceleration */
	calibrate_ACCEL();

}
