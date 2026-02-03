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
char STEP_STRING_BUFF[5];

/* Implementations */
void START_DISPLAY() {
	/* Constant char */
	char STEP_text[] = "Total Steps: 11111";
	char HR_text[] = "HR:";
	char BPM_text[] = " 111bpm";
	/* Step title */
	ssd1306_SetCursor(0, 12);
	ssd1306_WriteString(STEP_text, Font_7x10, White);
	/* HR titles */
	ssd1306_SetCursor(36, 40);
	ssd1306_WriteString(HR_text, Font_7x10, White);
	ssd1306_SetCursor(56, 40);
	ssd1306_WriteString(BPM_text, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void display_STEP(){
//	sprintf(STEP_STRING_BUFF,"%f", MagSamples[0].Magnitude);
	ssd1306_SetCursor(14, 1);
	ssd1306_UpdateScreen();
}

void display_HR() {
	ssd1306_SetCursor(9, 3);
	ssd1306_UpdateScreen();
}


