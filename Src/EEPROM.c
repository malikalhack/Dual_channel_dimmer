/**
 * @file    eeprom.c
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    09.07.2015 12:43:51
 * @date    19.11.2022
 * @details Functions for working with non-volatile EEPROM memory
 * @note    ATMega8
 */

/******************************* Included files *******************************/
#include "eeprom.h"
#include <avr/io.h>
/********************* Application Programming Interface **********************/
/* EEPROM read function */
BYTE eeprom_read(WORD Addr) {
	while(EECR & (1<<EEWE));
	EEAR = Addr;
	EECR |= (1<<EERE);
	return EEDR;
}
/*----------------------------------------------------------------------------*/
/* EEPROM write function */
void eeprom_write(WORD Addr, BYTE Data) {
	while(EECR & (1<<EEWE));
	EEAR = Addr;
	EEDR = Data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}
/******************************************************************************/
