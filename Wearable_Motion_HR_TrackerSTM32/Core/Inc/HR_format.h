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
#define DIE_TEMP_READY 0x01
#define FIFO_WR_PTR 0x04
#define FIFO_RD_PTR 0x06
#define OVF_COUNTER 0x05
#define FIFO_DATA 0x07
#define FIFO_CONFIG 0x08
#define MODE_CONFIG 0x09
#define SPO2_CONFIG 0x0A
#define LED1_PA 0x0C
#define TEMP_INT 0x1F
#define TEMP_FRAC 0x20
#define TEMP_EN 0x21

/* Externs */
extern int8_t MAX30102_die_temp[];

/* Function Declarations */
void MAX30102_init();

void MAX30102_temp();

#endif /* INC_HR_FORMAT_H_ */
