/************************************
* Title		: Keyboard.h			*
* Release	: 0.1.B					*
* Creator	: Malik					*
* Frequency	: 8 MHz (int)			*
* Created	: 27.07.2019 19:12:12	*
* Changed	: 22.10.2020			*
************************************/

#ifndef KEYBOARD_H_
#define KEYBOARD_H_
/******************************** Included files ********************************/
#include "standard.h"
/********************************** Definition **********************************/
#define BTNPORT		PORTC
#define BTNPIN		PINC
#define BTNDDR		DDRC
#define btn_up		0
#define btn_down	1
#define btn_left	2
#define btn_right	3
/******************************** Prototypes API ********************************/
void key_scan (void);
void key_action (void);
extern void EEPROM_write(WORD Addr, BYTE Data);
/********************************************************************************/
#endif /* KEYBOARD_H_ */
