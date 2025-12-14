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
MPU6050_Accelerometer START_VECTOR;
MPU6050_Accelerometer PEAK_VECTOR;
MPU6050_Accelerometer STOP_VECTOR;
MPU6050_Accelerometer PEAK_SERIES[PEAK_SERIES_SIZE];
/* INTERRUPT (Sample Acceleration >> Receive Magnitude) */

void vector_tracking() {

	/* Pass 1,2,3 magnitudes */
	switch (vector_state) {
		/* Start */
		case (0):
			/* Evaluate */
			if ((MagSamples[0].Magnitude > MagSamples[1].Magnitude)
				&& (MagSamples[1].Magnitude > MagSamples[2].Magnitude)
				&& (MagSamples[2].Magnitude > MagSamples[3].Magnitude)) {
				/* */
				START_VECTOR = MagSamples[3];
				STEP.BEGIN = clock();
				vector_state++;
			}
			break;
		/* Peak */
		case (1):
			/* Evaluate */
			if (IIR[sample_count-1].Magnitude >= PEAK_THRESHOLD) {
				/* Retains value across function calls */
				static uint8_t peak_count;
				/* Store up to ~200 (2 seconds) */
				PEAK_SERIES[peak_count] = IIR[sample_count-1];
			}
			else if (IIR[sample_count-1].Magnitude < PEAK_THRESHOLD) {
				/* Iterate & Return Max Value */
			}
			break;
		/* End(2) */
		case (2):
			/* Evaluate */
			if ((MagSamples[0].Magnitude > MagSamples[1].Magnitude)
				&& (MagSamples[1].Magnitude > MagSamples[2].Magnitude)) {
				/* */
				STOP_VECTOR = MagSamples[2];
				STEP.END = clock();
				STEP.TIME = STEP.END - STEP.BEGIN;
				vector_state++;
			}
			break;
	default:
		//code
	}
}
