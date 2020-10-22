/************************************
* Title		: main.c				*
* Release	: 0.1.B					*
* Creator	: Malik					*
* CPU		: ATMega8				*
* Frequency	: 8 MHz (int)			*
* Created	: 09.07.2015 12:43:51	*
* Changed	: 22.10.2020			*
************************************/
#ifndef MAIN_H
#define MAIN_H
/******************************** Included files ********************************/
#include "standard.h"
#include "EEPROM.h"
#include "LCD.h"
#include "Keyboard.h"
/******************************** Prototypes API ********************************/
ISR (TIMER2_COMP_vect);
/**********************************  Variables **********************************/
BYTE control = 0;			/* Control register */
volatile BYTE time = 125;	/* Variable setting the time */
BYTE choice = 1;			/* Channel selection variable */
BYTE setup[2] = {};			/* Control channels	*/
/********************************************************************************/
#endif /* MAIN_H */
