/**
 * @file    pwm.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 */

/******************************* Included files *******************************/
 #include "pwm.h"
 #include "EEPROM.h"
/********************* Application Programming Interface **********************/
 void pwm_init(struct setup_t * settings) {
    settings->ocr1 = eeprom_read(0x0000);
    settings->ocr2 = eeprom_read(0x0001);
 }
/******************************************************************************/
