/**
 * @file    eeprom.c
 * @version 2.0.0
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
byte_t eeprom_read(word_t Addr) {
    while(EECR & (1<<EEWE));
    EEAR = Addr;
    EECR |= (1<<EERE);
    return EEDR;
}
/*----------------------------------------------------------------------------*/
/* EEPROM write function */
void eeprom_write(word_t Addr, byte_t Data) {
     while(EECR & (1<<EEWE));
    EEAR = Addr;
    EEDR = Data;
    EECR |= (1<<EEMWE);
    EECR |= (1<<EEWE);
}
/******************************************************************************/
