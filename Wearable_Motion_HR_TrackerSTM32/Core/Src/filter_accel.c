/*
 * Process_ACCEL.c
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#include <filter_accel.h>

/* Global Variables */

/* INTERRUPT (Sample Acceleration) */

void sample_HPF_IT() {
	/* 1 / f(s)*2*3.14 */
	float RC = 1.0/(CUTOFF*2*M_PI);
	/* 1 / SAMPLE_RATE */
	float dt = 1.0 / 80;
	/* Coefficient */
	float alpha = RC/(RC + dt);

	if (sample_count <= NUM_SAMPLES-1) {

		/* Store sample[i] */
		Sampling[sample_count].X = Acceleration.X;
		Sampling[sample_count].Y = Acceleration.Y;
		Sampling[sample_count].Z = Acceleration.Z;

		/* Recursive equation */
		IIR[sample_count].X = alpha * (IIR[sample_count-1].X + Sampling[sample_count].X - Sampling[sample_count-1].X);
		IIR[sample_count].Y = alpha * (IIR[sample_count-1].Y + Sampling[sample_count].Y - Sampling[sample_count-1].Y);
		IIR[sample_count].Z = alpha * (IIR[sample_count-1].Z + Sampling[sample_count].Z - Sampling[sample_count-1].Z);

		/* Magnitude calculation */
		mag_ACCEL[sample_count] = (pow(IIR[sample_count].X, 2) + pow(IIR[sample_count].Y, 2) + pow(IIR[sample_count].Z, 2));
		mag_ACCEL[sample_count] = sqrt(mag_ACCEL[sample_count]);

		/* Increment */
		sample_count++;

	}

	else {

		/* Rolling IIR */
		IIR[0].X = IIR[NUM_SAMPLES-1].X;
		IIR[0].Y = IIR[NUM_SAMPLES-1].Y;
		IIR[0].Z = IIR[NUM_SAMPLES-1].Z;
		/* Rolling samples */
		Sampling[0].X = Sampling[NUM_SAMPLES-1].X;
		Sampling[0].Y = Sampling[NUM_SAMPLES-1].Y;
		Sampling[0].Z = Sampling[NUM_SAMPLES-1].Z;

		mag_ACCEL[0] = mag_ACCEL[NUM_SAMPLES-1];

		sample_count = 1;

		/* Clear */
		memset(&IIR[1], 0, sizeof(IIR[0]) * (NUM_SAMPLES - 1));
		memset(&Sampling[1], 0, sizeof(Sampling[0]) * (NUM_SAMPLES - 1));
		memset(&mag_ACCEL[1], 0, sizeof(mag_ACCEL[0]) * (NUM_SAMPLES - 1));

		/* Call */
		sprintf(UART, "Clear!");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen(UART), 100);
	}
}
