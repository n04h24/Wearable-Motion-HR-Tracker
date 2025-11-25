/*
 * Process_ACCEL.c
 *
 *  Created on: Nov 23, 2025
 *      Author: noah
 */

#include "clean_accel.h"

/* Receive (Calibrated) Acceleration on Polling or IT */

void high_pass_ACCEL(int num_samples){

	float RC = 1.0/(CUTOFF*2*M_PI);
	//#define CUTOFF
	float dt = 1.0 / 80;
	//Confirm SMPLRT_DIV
	float alpha = RC/(RC + dt);
	//Coefficient
	MPU6050_Accelerometer Sampling[num_samples];

	Sampling[0].X = 0;
	Sampling[0].Y = 0;
	Sampling[0].Z = 0;
	//Initial condition

	for (int i = 1; i < num_samples; i++){
//		Acceleration_IIR[i] = alpha * (Acceleration_IIR[i-1] + data.recordedSamples[i] - data.recordedSamples[i-1]);
		//y[i] = alpha * (y[i-1] + x[i] - x[i-1])
	}
}

/*
#if HIGH_PASS
{
    float RC = 1.0/(CUTOFF*2*3.14);
    float dt = 1.0/SAMPLE_RATE;
    float alpha = RC/(RC + dt);
    float filteredArray[numSamples];
    filteredArray[0] = data.recordedSamples[0];
    for (i = 1; i<numSamples; i++){
        filteredArray[i] = alpha * (filteredArray[i-1] + data.recordedSamples[i] - data.recordedSamples[i-1]);
    }
    data.recordedSamples = filteredArray;
}
#endif
*/
