#ifndef MPU6050_CONF_H
#define MPU6050_CONF_H

//Includes
#include "main.h"

//Extern
extern uint8_t check_memory;

//Function declaration
void config_I2Cmem(uint16_t device, uint16_t memory_add, uint8_t write_mem, uint16_t mem_size, uint16_t data_size);

#endif // MPU6050_CONF_H
