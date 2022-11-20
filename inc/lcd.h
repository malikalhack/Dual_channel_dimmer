/**
 * @file    lcd.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    09.07.2015 12:43:51
 * @date    19.11.2022
 * @note    8 MHz
 */

#ifndef LCD_H_
#define LCD_H_
/******************************* Included files *******************************/
#include "standard.h"
/*************** Application Programming Interface prototypes *****************/
void lcd_init (void);
void lcd_show (void);
void lcd_write (BYTE dat_com, BYTE data);
BYTE lcd_read (BYTE dat_com);
void lcd_convert (register BYTE znach);
/******************************************************************************/
#endif /* LCD_H_ */
