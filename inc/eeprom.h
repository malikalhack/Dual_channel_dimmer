/**
 * @file    eeprom.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    27.07.2019 18:44:16
 * @date    19.11.2022
 * @note    ATMega8
 */

#ifndef EEPROM_H
#define EEPROM_H
/******************************* Included files *******************************/
#include "standard.h"
/*************** Application Programming Interface prototypes *****************/
BYTE eeprom_read (WORD Addr);               ///< EEPROM read function
void eeprom_write(WORD Addr, BYTE Data);    ///< EEPROM write function
/******************************************************************************/
#endif /*EEPROM_H*/
