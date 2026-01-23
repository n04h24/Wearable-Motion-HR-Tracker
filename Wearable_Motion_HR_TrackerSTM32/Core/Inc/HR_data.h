/*
 * HR_data.h
 *
 *  Created on: Jan 19, 2026
 *      Author: noah
 */

#ifndef INC_HR_DATA_H_
#define INC_HR_DATA_H_

/* Includes */
#include "main.h"

/* Defines */
#define MODBUS_FRAME_WAIT 3.5 /* Char (10-bit) */
#define RTU_MSG_MAXLENGTH 14

/* Externs */
typedef struct {
	uint8_t Device_Address;
	uint8_t Funct_Code;
	uint8_t Reg_Add_A;
	uint8_t Reg_Add_B;
	uint8_t Reg_Num_A;
	uint8_t Reg_Num_B;
	uint16_t CRC_check;
} MODBUS_RTU_Transmit;

extern UART_HandleTypeDef huart1;
extern uint8_t MODBUS_WRITE_BUFFER[RTU_MSG_MAXLENGTH];
extern uint8_t MODBUS_READ_BUFFER[RTU_MSG_MAXLENGTH];

/* Function Declarations */
void MAX30102_collect();
void MAX30102_HR_SPO2();
void MAX30102_DIE_TEMP();
static uint16_t CRC_check(uint8_t *data, uint8_t len);

#endif /* INC_HR_DATA_H_ */
