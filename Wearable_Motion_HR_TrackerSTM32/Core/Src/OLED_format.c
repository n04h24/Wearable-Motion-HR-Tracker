/*
 * OLED_format.c
 *
 *  Created on: Jan 16, 2026
 *      Author: noah
 */
#include "OLED_format.h"


/* Includes */

/* Variable definitions */


/* Implementations */
void HR_DISPLAY() {

	ssd1306_SetCursor(0, 0);
//	ssd1306_WriteString(test_String, Font_7x10, White);
	ssd1306_UpdateScreen();
}


void STEP_DISPLAY(){
	ssd1306_SetCursor(0, 0);
//	ssd1306_WriteString(test_String, Font_7x10, White);
	ssd1306_UpdateScreen();
}


