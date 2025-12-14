/*
 * peak_detection.c
 *
 *  Created on: Dec 14, 2025
 *      Author: noah
 */

#include "peak_detection.h"

/* Global Variables */
uint8_t vector_state = 0; //EXTERN
double START;
double PEAK;
double END;

/* INTERRUPT (Sample Acceleration >> Receive Magnitude) */

void vector_tracking() {

	/* Retrieve 1,2,3 magnitudes */

	switch (vector_state) {
		/* Start */
		case (0):
			/* Evaluate */
			if (1) {

			}
			break;
		/* Peak */
		case (1):
			//code
			break;
		/* End/Start */
		case (2):
				//code
				break;
	default:
		//code
	}

}
