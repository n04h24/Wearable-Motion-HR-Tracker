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

/* ––––––––––––––––––––––––––––––––––––––– TEST FUNCTION */
// MAX30102
#define MAX30102_ADDR_7BIT   0x57
#define REG_PART_ID          0xFF
#define REG_REV_ID           0xFE
#define REG_INT_STATUS2      0x01

static HAL_StatusTypeDef read_reg(uint16_t dev_addr_8bit, uint8_t reg, uint8_t *val)
{
    *val = 0x00;
    return HAL_I2C_Mem_Read(&hi2c2,
                            dev_addr_8bit,
                            reg,
                            I2C_MEMADD_SIZE_8BIT,
                            val,
                            1,
                            100);
}

void I2C2_MAX30102_TestFunction();

void I2C2_MAX30102_FailureMode();

void I2C2_MAX30102_ACKing();

static uint8_t rd(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t reg)
	{
	    uint8_t v = 0;
	    HAL_I2C_Mem_Read(hi2c, addr, reg, I2C_MEMADD_SIZE_8BIT, &v, 1, 100);
	    return v;
	}

void I2C2_MAX30102_ReadRegisters();

#endif /* INC_HR_FORMAT_H_ */
