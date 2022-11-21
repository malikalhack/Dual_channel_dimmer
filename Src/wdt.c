/**
 * @file    WDT.c
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    06.05.2020
 * @date    21.11.2022
 * @note    MCU:   ATmega8
 */

/****************************** Included files ********************************/
#include <avr/io.h>
#include "WDT.h"
/*************************** Dispatcher functions *****************************/
/** @fn throw_bone */
void wd_throw_bone (void * ptr) {
    asm ("WDR");
}
/************************************* API ************************************/
/** @fn wdt_init */
void wdt_init(const BYTE cycles) {
    switch(cycles) {
        case WD_32K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP0);
            break;
        case WD_64K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP1);
            break;
        case WD_128K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP1) + BIT(WDP0);
            break;
        case WD_256K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP2);
            break;
        case WD_512K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP2) + BIT(WDP0);
            break;
        case WD_1024K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP2) + BIT(WDP1);
            break;
        case WD_2048K_CYCLES:
            WDTCR = BIT(WDCE) + BIT(WDE);
            WDTCR = BIT(WDP2) + BIT(WDP1) + BIT(WDP0);
        case WD_16K_CYCLES:
        default: break;
    }
}
/******************************************************************************/
/** @fn wdt_start */
void wdt_start(void) {
    WDTCR |= BIT(WDCE) + BIT(WDE);     /* WatchDog enable */
}
/******************************************************************************/
/** @fn wdt_stop */
void wdt_stop(void) {
    asm ("WDR");
    WDTCR |= BIT(WDCE) + BIT(WDE);
    WDTCR = 0;
}
/******************************************************************************/
