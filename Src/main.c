/**
 * @file    main.c
 * @version 2.0.0
 * @authors Anton Chernov
 * @date    09.07.2015 12:43:51
 * @date    22.10.2020
 * @details Two-channel dimmer control program with display of brightness level on LCD.
 */

/******************************* Included files *******************************/
#include "TaskManager.h"
#include "bsp.h"
#include "pwm.h"
#include "keyboard.h"
#include "wdt.h"
/**************************** Private  variables ******************************/
static volatile WORD SysTime;    /* Time counter */;
/********************************* Entry point ********************************/
int main(void) {
    struct setup_t settings;
    pwm_init(&settings);
    bsp_config(&settings, &SysTime);
    tm_init(&SysTime);
    key_init();
    wdt_init(WD_1024K_CYCLES);
    start_pwm();

    return tm_run();
}
/********************************************************************************/
