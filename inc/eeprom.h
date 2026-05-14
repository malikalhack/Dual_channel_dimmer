/**
 * @file    eeprom.h
 * @version 2.0.0
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
byte_t eeprom_read (word_t Addr);               ///< EEPROM read function
void eeprom_write(word_t Addr, byte_t Data);    ///< EEPROM write function
/******************************************************************************/
#endif /*EEPROM_H*/
