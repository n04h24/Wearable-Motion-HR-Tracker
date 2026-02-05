/*
 * OLED_format.c
 *
 *  Created on: Jan 16, 2026
 *      Author: noah
 */
/* Includes */
#include "OLED_format.h"

/* Variable definitions */
char HR_STRING_BUFF[9];
char STEPS_TAKEN[5];

/* Implementations */
void START_DISPLAY() {
	/* Constant char */
	char STEP_text[] = "Total Steps: ";
	char HR_text[] = "HR:";
	/* Step title */
	ssd1306_SetCursor(0, 12);
	ssd1306_WriteString(STEP_text, Font_7x10, White);
	/* HR titles */
	ssd1306_SetCursor(36, 40);
	ssd1306_WriteString(HR_text, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void display_STEP(){
	snprintf(STEPS_TAKEN, 5, "%" PRIu16, TOTAL_STEPS);
	ssd1306_SetCursor(86, 12);
	ssd1306_WriteString(STEPS_TAKEN, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void display_HR() {
	uint8_t STR_INCREMENT;
	if (HR_POINTER != 0) {
		STR_INCREMENT = 1;
	}
	else {
		STR_INCREMENT = 0;
	}
	uint8_t TEMP_HR_STR = HEARTBEAT_BUFF[HR_POINTER-STR_INCREMENT];
	snprintf(HR_STRING_BUFF, sizeof(HR_STRING_BUFF), "%ubpm  ", TEMP_HR_STR);
	ssd1306_SetCursor(58, 40);
	ssd1306_WriteString(HR_STRING_BUFF, Font_7x10, White);
	ssd1306_UpdateScreen();
}


