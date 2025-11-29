/*
 * Process_ACCEL.c
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#include <filter_accel.h>

MPU6050_Accelerometer Sampling[NUM_SAMPLES];
double mag_ACCEL[NUM_SAMPLES-1]; //-1 ∵ Discard IIR[0] = {0,0,0}

/* INTERRUPT (Sample Acceleration) */

void sample_HPF(){

	/* 1 / f(s)*2*3.14 */
	float RC = 1.0/(CUTOFF*2*M_PI);
	/* 1 / SAMPLE_RATE */
	float dt = 1.0 / 80;
	/* Coefficient */
	float alpha = RC/(RC + dt);
	/* IIR & Sampling ARRAY storage */
	MPU6050_Accelerometer IIR[NUM_SAMPLES];
	/* Retrieve 1st Sample (i-1) */
	convert_ACCEL();
	/* Initial conditions (equation) */
	IIR[0].X = 0;
	IIR[0].Y = 0;
	IIR[0].Z = 0;
	/* Store 1st Sample[0] */
	Sampling[0].X = Acceleration.X;
	Sampling[0].Y = Acceleration.Y;
	Sampling[0].Z = Acceleration.Z;

	for (int i = 1; i < NUM_SAMPLES; i++){
		/* Retrieve current XYZ */
		convert_ACCEL();
		/* Store (iteratively) */
		Sampling[i].X = Acceleration.X;
		Sampling[i].Y = Acceleration.Y;
		Sampling[i].Z = Acceleration.Z;
		/* IIR equation (recursive relation) */
		IIR[i].X = alpha * (IIR[i-1].X + Sampling[i].X - Sampling[i-1].X);
		IIR[i].Y = alpha * (IIR[i-1].Y + Sampling[i].Y - Sampling[i-1].Y);
		IIR[i].Z = alpha * (IIR[i-1].Z + Sampling[i].Z - Sampling[i-1].Z);
	}
	/* Copy (from 1st element) IIR into Sampling */
	memcpy(Sampling, &IIR[1], (NUM_SAMPLES-1) * sizeof(IIR[0]));
}

void euclidean_NORMS() {
	for (int i = 0; i < NUM_SAMPLES-1; i++) {
		/* magnitude^2 =  x^2 + y^2 + z^2 */
		mag_ACCEL[i] = (pow(Sampling[i].X, 2) + pow(Sampling[i].Y, 2) + pow(Sampling[i].Z, 2));
		/* magnitude = √magnitude */
		mag_ACCEL[i] = sqrt(mag_ACCEL[i]);
	}
}

void process_ACCEL() {

	sample_HPF();

	euclidean_NORMS();

}
