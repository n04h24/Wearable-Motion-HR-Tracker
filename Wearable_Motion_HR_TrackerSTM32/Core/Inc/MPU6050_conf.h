#ifndef MPU6050_CONF_H
#define MPU6050_CONF_H

/* Includes */
#include "main.h"
#include <math.h>
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
//Universal register value buffer

typedef struct  {

	float Xa;
	float Ya;
	float Za;

} FactoryTrims;
//Store internal characteristics
extern FactoryTrims FT;
//Initialise as 'FT'


typedef struct {

	float x;
	float y;
	float z;

} Offsets;
//Store offsets from calibration routine
extern Offsets acceleration_offs;
//Initialise as 'acceleration'

extern uint8_t ACCEL_X_H;
extern uint8_t ACCEL_X_L;
extern uint8_t ACCEL_Y_H;
extern uint8_t ACCEL_Y_L;
extern uint8_t ACCEL_Z_H;
extern uint8_t ACCEL_Z_L;
//Temporary high & low 8-bit storage

/* Function Declarations */
void config_I2Cmem(uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size);

void calc_FTa();

void check_accel();

void startSFT();

void accel_calibration(struct Offsets *acceleration);

#endif // MPU6050_CONF_H
