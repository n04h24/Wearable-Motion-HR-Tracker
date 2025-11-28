#ifndef MPU6050_CONF_H
#define MPU6050_CONF_H

/* Includes */
#include "main.h"
#include <inttypes.h>

/* Defines */
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

/* Externs */
extern uint8_t check_memory;
extern MPU6050_Accelerometer Offsets;
extern MPU6050_Accelerometer FT;

extern uint8_t ACCEL_X_H;
extern uint8_t ACCEL_X_L;
extern uint8_t ACCEL_Y_H;
extern uint8_t ACCEL_Y_L;
extern uint8_t ACCEL_Z_H;
extern uint8_t ACCEL_Z_L;

/* Function Declarations */
void config_I2Cmem(uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size);

void calculate_FACT();

void test_RESPONSE();

void readA_CONCAT(int16_t *raw_X, int16_t *raw_Y, int16_t *raw_Z);

void calculate_OFFS();

void convert_ACCEL();

void MPU6050_init();

#endif // MPU6050_CONF_H
