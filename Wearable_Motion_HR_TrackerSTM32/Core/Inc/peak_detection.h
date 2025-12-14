/*
 * peak_detection.h
 *
 *  Created on: Dec 14, 2025
 *      Author: noah
 */

#ifndef INC_PEAK_DETECTION_H_
#define INC_PEAK_DETECTION_H_

/* Includes */
#include "main.h"
#include "filtering_accel.h"
#include <time.h>
/* Defines */
#define STEP_THRESHOLD 0.35
/* Externs */
typedef struct {
	float BEGIN;
	float STOP;
	float TIME;
} Timing;
extern float step_TIME;
extern float step_BEGIN;
extern float step_STOP;
extern uint8_t vector_state;
extern Timing STEP;
extern MPU6050_Accelerometer START;
extern MPU6050_Accelerometer PEAK;
extern MPU6050_Accelerometer END;
/* Function Prototypes */
void vector_tracking();

#endif /* INC_PEAK_DETECTION_H_ */
