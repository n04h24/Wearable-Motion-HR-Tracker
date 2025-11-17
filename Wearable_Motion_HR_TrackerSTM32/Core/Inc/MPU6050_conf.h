#ifndef MPU6050_CONF_H
#define MPU6050_CONF_H

//Includes
#include "main.h"
#include <math.h>

//Extern
extern uint8_t check_memory;

//Function declaration
void config_I2Cmem(char *buffer, size_t size_buff, uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size);

void self_testXYZ(char *buffer, size_t size_buff);

#endif // MPU6050_CONF_H
