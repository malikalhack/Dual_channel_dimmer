/************************************
* Title		: EEPROM.h				*
* Release	: 0.1.B					*
* Creator	: Malik					*
* CPU		: ATMega8				*
* Created	: 27.07.2019 18:44:16	*
* Changed	: 22.10.2020			*
************************************/
#ifndef EEPROM_H
#define EEPROM_H
/******************************** Prototypes API ********************************/
BYTE EEPROM_read (WORD Addr);				/* EEPROM read function */
void EEPROM_write(WORD Addr, BYTE Data);	/* EEPROM write function */
/********************************************************************************/
#endif /*EEPROM_H*/
