/*
 * Process_ACCEL.c
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#include "clean_accel.h"

/* Receive (Calibrated) Acceleration on Polling or IT */

void filter_ACCEL(){

	/* 1 / f(s)*2*3.14 */
	float RC = 1.0/(CUTOFF*2*M_PI);
	/* 1 / SAMPLE_RATE */
	float dt = 1.0 / 80;
	/* Coefficient */
	float alpha = RC/(RC + dt);
	/* IIR & Sampling ARRAY storage */
	MPU6050_Accelerometer IIR[NUM_SAMPLES];

	/* Initial condition (equation) */
	IIR[0].X = 0;
	IIR[0].Y = 0;
	IIR[0].Z = 0;
	/* Retrieve 1st Sample (i-1) */
	convert_ACCEL(1);
	/* Store (i-1) */
	Sampling[0].X = Acceleration.X;
	Sampling[0].Y = Acceleration.Y;
	Sampling[0].Z = Acceleration.Z;


	for (int i = 1; i < NUM_SAMPLES; i++){

		/* Retrieve current XYZ */
		convert_ACCEL(1);

		/* Store (iteratively) */
		Sampling[i].X = Acceleration.X;
		Sampling[i].Y = Acceleration.Y;
		Sampling[i].Z = Acceleration.Z;

//		/* IIR equation (recursive relation) */
		IIR[i].X = alpha * (IIR[i-1].X + Sampling[i].X - Sampling[i-1].X);
		IIR[i].Y = alpha * (IIR[i-1].Y + Sampling[i].Y - Sampling[i-1].Y);
		IIR[i].Z = alpha * (IIR[i-1].Z + Sampling[i].Z - Sampling[i-1].Z);

	}

	/* Store filtered data in 'Sampling' for processing */
	Sampling->Z = IIR->Z;
	Sampling->Y = IIR->Y;
	Sampling->X = IIR->X;
}

