/*
 * Process_ACCEL.c
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#include <filtering_accel.h>

/* Global Variables */
MPU6050_Accelerometer IIR[NUM_SAMPLES];
MPU6050_Accelerometer Sampling[NUM_SAMPLES];
MPU6050_Accelerometer MagSamples[4];
uint8_t sample_count = 1;

/* INTERRUPT (Sample Acceleration) */

void conditions_INIT() {

	IIR[0].X = 0;
	IIR[0].Y = 0;
	IIR[0].Z = 0;
	IIR[0].Magnitude = 0;

	Sampling[0].X = 0;
	Sampling[0].Y = 0;
	Sampling[0].Z = 0;
	Sampling[0].Magnitude = 0;
}

void HPF_magnitiude_IT() {

	/* IIR variables */
	float RC = 1.0/(CUTOFF*2*M_PI);
	float dt = 1.0 / 80;
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
		IIR[sample_count].Magnitude = (pow(IIR[sample_count].X, 2) + pow(IIR[sample_count].Y, 2) + pow(IIR[sample_count].Z, 2));
		IIR[sample_count].Magnitude = sqrt(IIR[sample_count].Magnitude);
		/* Increment */
		sample_count++;
	}

	else {
		/* Rolling IIR & Magnitude */
		IIR[0].X = IIR[NUM_SAMPLES-1].X;
		IIR[0].Y = IIR[NUM_SAMPLES-1].Y;
		IIR[0].Z = IIR[NUM_SAMPLES-1].Z;
		IIR[0].Magnitude = IIR[NUM_SAMPLES-1].Magnitude;
		/* Rolling samples */
		Sampling[0].X = Sampling[NUM_SAMPLES-1].X;
		Sampling[0].Y = Sampling[NUM_SAMPLES-1].Y;
		Sampling[0].Z = Sampling[NUM_SAMPLES-1].Z;
		/* Reset count */
		sample_count = 1;
		/* Clear */
		memset(&IIR[1], 0, sizeof(IIR[0]) * (NUM_SAMPLES - 1));
		memset(&Sampling[1], 0, sizeof(Sampling[0]) * (NUM_SAMPLES - 1));
		/* Call */
		sprintf(UART, "Clear!");
		HAL_UART_Transmit(&huart2, (uint8_t*)UART, strlen(UART), 100);
	}

	/* FIFO */
	MagSamples[3] = MagSamples[2];
	MagSamples[2] = MagSamples[1];
	MagSamples[1] = MagSamples[0];
	/* Pushes sample_count back (1) after incremented */
	MagSamples[0] = IIR[sample_count-1];
}
