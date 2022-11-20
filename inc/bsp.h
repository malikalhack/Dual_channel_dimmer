/**
 * @file    bsp.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 * @note ATMega8 - 8 MHz
 */

#ifndef BSP_H
#define BSP_H
/******************************* Included files *******************************/
#include "standard.h"
/********************************* Definition *********************************/
struct setup_t {
    BYTE ocr1;
    BYTE ocr2;
};
/*************** Application Programming Interface prototypes *****************/
void bsp_config(struct setup_t*);
/******************************************************************************/
#endif /* !BSP_H */
