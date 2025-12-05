/*
 * Process_ACCEL.h
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#ifndef INC_SAMPLE_ACCEL_H_
#define INC_SAMPLE_ACCEL_H_

/* Includes */
#include "main.h"
#include "MPU6050_conf.h"

/* Defines */
#define NUM_SAMPLES 80
#define CUTOFF 1 //100Hz for SMPLRT >> 50Hz for Attenuation

/* Externs */
extern MPU6050_Accelerometer IIR[NUM_SAMPLES]; // EXTERNAL
extern MPU6050_Accelerometer Sampling[NUM_SAMPLES]; // EXTERNAL
extern double mag_ACCEL[NUM_SAMPLES];
extern uint8_t sample_count; // EXTERNAL

/* Function Declarations */
void conditions_INIT();

void HPF_magnitiude_IT();

#endif /* INC_SAMPLE_ACCEL_H_ */
