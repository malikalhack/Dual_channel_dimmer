/**
 * @file    bsp.h
 * @version 2.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 * @note    MCU:   ATmega8
 * @note    Clock: 8 MHz
 */

#ifndef BSP_H
#define BSP_H
/******************************* Included files *******************************/
#include "standard.h"
/********************************* Definition *********************************/
#define BTN_UP      (0)
#define BTN_DOWN    (1)
#define BTN_LEFT    (2)
#define BTN_RIGHT   (3)

struct setup_t {
    BYTE ocr1;
    BYTE ocr2;
};
/*************** Application Programming Interface prototypes *****************/
void bsp_config(struct setup_t*, volatile WORD*);
void bsp_start_pwm(void);
void bsp_stop_pwm(void);
void bsp_change_pwm(BYTE, BYTE);
void bsp_lcd_write(BYTE, BYTE);
BYTE bsp_lcd_read(BYTE);
BYTE bsp_get_ocr1(void);
BYTE bsp_get_ocr2(void);
BYTE bsp_get_pins(void);
void bsp_speed_up_systime(void);
void bsp_normalize_systime(void);
/******************************************************************************/
#endif /* !BSP_H */
