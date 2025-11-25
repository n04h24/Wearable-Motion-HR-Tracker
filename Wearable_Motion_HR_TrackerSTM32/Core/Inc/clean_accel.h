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
#include <math.h>

/* Defines */
#define CUTOFF 1

/* Externs */

/* Function Declarations */
void filter_ACCEL();

#endif /* INC_CLEAN_ACCEL_H_ */
