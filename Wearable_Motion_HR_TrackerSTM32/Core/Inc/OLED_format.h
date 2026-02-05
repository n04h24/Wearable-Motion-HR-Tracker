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
#include "filtering_accel.h"
#include "peak_detection.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "math.h"
#include "inttypes.h"

/* Defines */

/* Extern */
extern char HR_STRING_BUFF[7];
extern char STEP_STRING_BUFF[5];

/* Function Declarations */
void START_DISPLAY();
void display_STEP();
void display_HR();

#endif /* SRC_OLED_FORMAT_H_ */
