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
#define MODBUS_DEV_ADDRESS 0x20

/* Externs */
/* typedef struct {
	uint8_t Device_Address;
	uint8_t Funct_Code;
	uint16_t Reg_Address;
	uint16_t Reg_Number;
	uint16_t CRC_check;
} MODBUS_RTU_Transmit; */

extern UART_HandleTypeDef huart1;
extern uint8_t MODBUS_WRITE_BUFFER[RTU_MSG_MAXLENGTH];
extern uint8_t MODBUS_READ_BUFFER[RTU_MSG_MAXLENGTH];

/* Function Declarations */
static void MODBUS_RTU_format(uint8_t *buffer, uint8_t funct_code);
void MAX30102_collect();
void MAX30102_HR_SPO2();
void MAX30102_DIE_TEMP();
static uint16_t CRC_check(uint8_t *data, uint8_t len);

#endif /* INC_HR_DATA_H_ */
