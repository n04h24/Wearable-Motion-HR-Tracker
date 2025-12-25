/*
 * heart_rate_read.h
 *
 *  Created on: Dec 17, 2025
 *      Author: noah
 */

#ifndef INC_HR_FORMAT_H_
#define INC_HR_FORMAT_H_

/* Includes */
#include "main.h"
#include "MPU6050_conf.h"

/* Defines */
#define MAX30102_ADD 0b1010111
#define MAX30102_WRITE 0xAE
#define MAX30102_READ 0xAF
#define FIFO_WR_PTR 0x04
#define FIFO_RD_PTR 0x06
#define OVF_COUNTER 0x05
#define FIFO_DATA 0x07
#define FIFO_CONFIG 0x08
#define MODE_CONFIG 0x09
#define SPO2_CONFIG 0x0A

/* Externs */

/* Function Declarations */
void MAX30102_config();

void MAX30102_HR();

#endif /* INC_HR_FORMAT_H_ */
