/*
 * Process_ACCEL.h
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#ifndef INC_CLEAN_ACCEL_H_
#define INC_CLEAN_ACCEL_H_

/* Includes */
#include "main.h"
#include "MPU6050_conf.h"

/* Defines */
#define CUTOFF 25 //100Hz for SMPLRT >> 50Hz for Attenuation
#define NUM_SAMPLES 200

/* Externs */
extern MPU6050_Accelerometer Sampling[NUM_SAMPLES];

/* Function Declarations */
void sample_HPF();

#endif /* INC_CLEAN_ACCEL_H_ */
