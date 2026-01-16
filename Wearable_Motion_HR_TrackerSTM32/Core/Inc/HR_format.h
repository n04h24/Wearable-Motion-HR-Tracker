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
#define INT_STAT1 0x00
#define INT_STAT2 0x01
#define INT_EN1 0x02
#define INT_EN2 0x03
#define FIFO_WR_PTR 0x04
#define FIFO_RD_PTR 0x06
#define OVF_COUNTER 0x05
#define FIFO_DATA 0x07
#define FIFO_CONFIG 0x08
#define MODE_CONFIG 0x09
#define SPO2_CONFIG 0x0A
#define LED1_PA 0x0C
#define LED2_PA 0x0D
#define TEMP_INT 0x1F
#define TEMP_FRAC 0x20
#define TEMP_EN 0x21
#define PART_ID 0xFF

/* Externs */
extern uint8_t check_memoryMAX;
extern uint8_t die_sample;
extern float MAX30102_DieTemp[20];

/* Function Declarations */
void MAX30102_init();

void MAX30102_temp();

void MAX30102_HR();

#endif /* INC_HR_FORMAT_H_ */
