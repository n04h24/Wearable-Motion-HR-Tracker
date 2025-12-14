/*
 * peak_detection.c
 *
 *  Created on: Dec 14, 2025
 *      Author: noah
 */

#include "peak_detection.h"

/* Global Variables */

uint8_t vector_state = 0; //EXTERN
Timing STEP;
MPU6050_Accelerometer START;
MPU6050_Accelerometer PEAK;
MPU6050_Accelerometer END;
/* INTERRUPT (Sample Acceleration >> Receive Magnitude) */

void vector_tracking() {

	/* Pass 1,2,3 magnitudes */
	switch (vector_state) {
		/* Start */
		case (0):
			/* Evaluate */
			if ((MagSamples.A > MagSamples.B) && (MagSamples.B > MagSamples.C)) {
				/* Store 1st */
				START = MagComponents[2];
				STEP.BEGIN = clock();
				vector_state++;
			}
			break;
		/* Peak */
		case (1):
//			if (1)
			break;
		/* End/Start */
		case (2):
				//code
				break;
	default:
		//code
	}

}
