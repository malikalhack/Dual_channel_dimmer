/************************************
* Title		: Keyboard.c			*
* Release	: 0.1.B					*
* Creator	: Malik					*
* Frequency	: 8 MHz (int)			*
* Created	: 27.07.2019 19:12:12	*
* Changed	: 22.10.2020			*
************************************/
/********************************************************************************/
/******************************** Included files ********************************/
#include "Keyboard.h"
/**********************************  Variables **********************************/
BYTE key, btn_speed = 50;
extern BYTE choice;			/* Channel select variable */
extern BYTE control;		/* Control register */
extern BYTE setup[2];		/* Control channels */
/************************************** API *************************************/
void key_scan (void) {
	register BYTE pin = 0;
	key = 0;
	static BYTE anti_dr;
	pin = BTNPIN|0xf0;
	if (pin != 0xff) {
		if (++anti_dr == 5) key = ~pin;
		if (anti_dr == btn_speed) {
			anti_dr = 0;
			btn_speed = 10;
		}
	}
	else {
		if (control&BIT(3)) {
			if (choice == 1) 	{
				EEPROM_write(0x0000, setup[choice - 1]);
			}
			else {
				EEPROM_write(0x0001, setup[choice - 1]);
			}
		}
		btn_speed = 50;
		anti_dr = 0;
	}
	return;
}
/********************************************************************************/
void key_action (void) {
	if (key) {
		if (key&BIT(btn_down)) {
			if (setup[choice-1] >= 5) {
				setup[choice-1] -= 5;
				control |= BIT(3);
				control &= ~(BIT(5)|BIT(4));
				if(choice-1) OCR1B=setup[1];
				else OCR1A = setup[0];
			}
			return;
		}
		if (key&BIT(btn_up)) {
			if (setup[choice-1] <= 245) {
				setup[choice-1] += 5;
				control |= BIT(3);
				control &= ~(BIT(5)|BIT(4));
				if(choice-1) OCR1B = setup[1];
				else OCR1A = setup[0];
			}
			return;
		}
		if (key&BIT(btn_right)) {
			if (choice < 2) {
				choice++;
				control &= ~(BIT(5)|BIT(4));
			}
			return;
		}
		if (key&BIT(btn_left)) {
			if (choice > 1) {
				choice--;
				control &= ~(BIT(5)|BIT(4));
			}
			return;
		}
	}
	return;
}
/********************************************************************************/
