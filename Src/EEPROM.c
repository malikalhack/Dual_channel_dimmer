/************************************
* Title		: EEPROM.c				*
* Release	: 0.1.B					*
* Creator	: Malik					*
* CPU		: ATMega8				*
* Created	: 09.07.2015 12:43:51	*
* Changed	: 22.10.2020			*
************************************/
/* Functions for working with non-volatile EEPROM memory */
/******************************** Included files ********************************/
#include "standard.h"
/************************************** API *************************************/
/* EEPROM read function */
BYTE EEPROM_read(WORD Addr) {
	while(EECR & (1<<EEWE));
	EEAR = Addr;
	EECR |= (1<<EERE);
	return EEDR;
}
/********************************************************************************/
/* EEPROM write function */
void EEPROM_write(WORD Addr, BYTE Data) {
	while(EECR & (1<<EEWE));
	EEAR = Addr;
	EEDR = Data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}
/********************************************************************************/
