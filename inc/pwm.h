/**
 * @file    pwm.h
 * @version 2.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 */

#ifndef PWM_H
#define PWM_H
/******************************* Included files *******************************/
#include "bsp.h"
/*************** Application Programming Interface prototypes *****************/
void pwm_init(struct setup_t *);
void start_pwm(void);
/******************************************************************************/
#endif /* !PWM_H */
