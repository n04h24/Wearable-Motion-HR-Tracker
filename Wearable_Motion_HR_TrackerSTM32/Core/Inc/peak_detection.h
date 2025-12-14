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
/* Defines */
#define STEP_THRESHOLD 0.35

/* Externs */
extern uint8_t vector_state;
extern double START;
extern double PEAK;
extern double END;

/* Function Prototypes */
void vector_tracking();

#endif /* INC_PEAK_DETECTION_H_ */
