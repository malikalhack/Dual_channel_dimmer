/************************************
* Title		: LCD.h					*
* Release	: 0.1.B					*
* Creator	: Malik					*
* Frequency	: 8 MHz (int)			*
* Created	: 09.07.2015 12:43:51	*
* Changed	: 22.10.2020			*
************************************/
#ifndef LCD_H_
#define LCD_H_
#define F_CPU 8000000UL
//=============================== Included files =================================
#include "standard.h"
#include <util/delay.h>
//================================= Definition ===================================

#define IPORT PORTD
#define IPIN PIND
#define IDDR DDRD
#define _COMM	0
#define _DATA	1
#define RS		0
#define RW		1
#define E		2
#define DB4		4
#define DB5		5
#define DB6		6
#define DB7		7
#define IB		0
#define BF		7
/* Display Symbols */
#define f_ya	10
#define f_r		11
#define f_km	12
#define f_o		13
#define f_s		14
#define f_t		15
#define f_mz	16
#define f_kb	17
#define f_a		18
#define f_n		19
#define f_l		20
#define f_proc	21
#define f_nomer	22
#define f_prob	23
#define f_sh	24
#define f_i		25
#define f_m		26
/******************************** Prototypes API ********************************/
void LCD_init (void);
void LCD_show (void);
void LCD_write (BYTE dat_com, BYTE data);
BYTE LCD_read (BYTE dat_com);
void convert (register BYTE znach);
/********************************************************************************/
#endif /* LCD_H_ */
