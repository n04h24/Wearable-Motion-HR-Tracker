/*
 * Process_ACCEL.h
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#ifndef INC_FILTER_ACCEL_H_
#define INC_FILTER_ACCEL_H_

/* Includes */
#include "main.h"
#include "MPU6050_conf.h"

/* Defines */
#define CUTOFF 25 //100Hz for SMPLRT >> 50Hz for Attenuation
#define NUM_SAMPLES 200

/* Externs */
extern MPU6050_Accelerometer Sampling[NUM_SAMPLES]; //Includes initial conditions
extern double mag_ACCEL[NUM_SAMPLES-1]; //-1 ∵ Discard IIR[0] = {0,0,0}

/* Function Declarations */
void sample_HPF();
void euclidean_NORMS();
void process_ACCEL();

#endif /* INC_FILTER_ACCEL_H_ */
