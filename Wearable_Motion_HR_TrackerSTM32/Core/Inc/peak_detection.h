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
#define PEAK_THRESHOLD 0.6
#define PEAK_SERIES_SIZE 200
/* Externs */
typedef struct {
	uint32_t BEGIN;
	uint32_t END;
	uint32_t TIME;
} Timing;
extern float step_TIME;
extern float step_BEGIN;
extern float step_STOP;
extern uint8_t vector_state;
extern Timing STEP;
extern MPU6050_Accelerometer START_VECTOR;
extern MPU6050_Accelerometer PEAK_VECTOR;
extern MPU6050_Accelerometer STOP_VECTOR;
extern MPU6050_Accelerometer PEAK_SERIES[PEAK_SERIES_SIZE];
/* Function Prototypes */
void vector_tracking();

#endif /* INC_PEAK_DETECTION_H_ */
