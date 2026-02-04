/*
 * OLED_format.c
 *
 *  Created on: Jan 16, 2026
 *      Author: noah
 */
/* Includes */
#include "OLED_format.h"

/* Variable definitions */
char HR_STRING_BUFF[3];
char STEPS_TAKEN[2];

/* Implementations */
void START_DISPLAY() {
	/* Constant char */
	char STEP_text[] = "Total Steps: ";
	char HR_text[] = "HR:";
	char BPM_text[] = " bpm";
	/* Step title */
	ssd1306_SetCursor(0, 12);
	ssd1306_WriteString(STEP_text, Font_7x10, White);
	/* HR titles */
	ssd1306_SetCursor(36, 40);
	ssd1306_WriteString(HR_text, Font_7x10, White);
	ssd1306_SetCursor(80, 40);
	ssd1306_WriteString(BPM_text, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void display_STEP(){
	snprintf(STEPS_TAKEN, 2, "1");
	ssd1306_SetCursor(58, 40);
	ssd1306_WriteString(STEPS_TAKEN, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void display_HR() {
	ssd1306_SetCursor(9, 3);
	ssd1306_UpdateScreen();
}


