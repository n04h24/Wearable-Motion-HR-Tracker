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
#define COLLECT_DATA 0x06
#define COLLECT_DATA_REGADD_A 0x00
#define COLLECT_DATA_REGADD_B 0x10
#define COLLECT_REG_NUM_START_A 0x00 /* "Valid Bytes" (Receive) */
#define COLLECT_REG_NUM_START_B 0x01
#define COLLECT_REG_NUM_STOP_A 0x00 /* "Data" (Receive) */
#define COLLECT_REG_NUM_STOP_B 0x02

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
static void MODBUS_send_buffer(uint8_t *buffer, uint8_t funct_code, uint8_t reg_addA, uint8_t reg_addB, uint8_t reg_numA, uint8_t reg_numB);
void MAX30102_collect();
void MAX30102_HR_SPO2();
void MAX30102_DIE_TEMP();
static uint16_t CRC_check(uint8_t *data, uint8_t len);

#endif /* INC_HR_DATA_H_ */
