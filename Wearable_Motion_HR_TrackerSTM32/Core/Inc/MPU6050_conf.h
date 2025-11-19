#ifndef MPU6050_CONF_H
#define MPU6050_CONF_H

/* Includes */
#include "main.h"
#include <math.h>

/* Defines */

/* Externs */
extern uint8_t check_memory;

/* Function Declarations */
void config_I2Cmem(char *buffer, size_t size_buff, uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size);

void calc_FTa(char *buffer, size_t size_buff);

void check_accel(char *buffer, size_t size_buff);

#endif // MPU6050_CONF_H
