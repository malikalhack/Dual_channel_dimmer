/**
 * @file    main.c
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    09.07.2015 12:43:51
 * @date    19.10.2020
 * @details Two-channel dimmer control program with display of brightness level on LCD.
 */

/******************************* Included files *******************************/
#include "TaskManager.h"
#include "bsp.h"
#include "pwm.h"

//#include "LCD.h"
//#include "Keyboard.h"
/********************************* Definition *********************************/
enum descriptors_t {
    FORBIDDEN_DESCRIPTOR = 0,
    DISPLAY,
    KEYBOARD,
    WATCHDOG,
    MAX_DESCRIPTOR
};

/**************************** Private  variables ******************************/
static volatile WORD SysTime;    /* Time counter */;
/********************************* Entry point ********************************/
int main(void) {
    struct setup_t settings;
    pwm_init(&settings);
    bsp_config(&settings);
    tm_init(&SysTime);
    lcd_init();

    //tm_add_task(KEYBOARD,);
    //tm_add_task(WATCHDOG,);

    while(1) {
        int tm_run (void);
    }
}
/***************************** Private functions ********************************/

/********************************************************************************/
