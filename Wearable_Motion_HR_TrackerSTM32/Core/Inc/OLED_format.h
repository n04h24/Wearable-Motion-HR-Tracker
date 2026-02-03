/*
 * OLED_format.h
 *
 *  Created on: Jan 16, 2026
 *      Author: noah
 */

#ifndef SRC_OLED_FORMAT_H_
#define SRC_OLED_FORMAT_H_

/* Includes */
#include "HR_data.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/* Defines */

/* Extern */
extern char HR_STRING_BUFF[SAMPLE_HR_COUNT];
/* Function Declarations */
void HR_DISPLAY();
void STEP_DISPLAY();

#endif /* SRC_OLED_FORMAT_H_ */
