/**
 * @file    bsp.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 * @note    ATMega8 - 8 MHz
 */

/******************************* Included files *******************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bsp.h"
/********************************* Definition *********************************/
#define _COMM	0
#define _DATA	1
#define RS		0
#define RW		1
#define E		2
#define DB4		4
#define DB5		5
#define DB6		6
#define DB7		7
#define IB		0
#define BF		7
/************************ Private  functions prototypes ***********************/
ISR (TIMER2_COMP_vect);
static void GPIOconfig(void);
static void TimersConfig(struct setup_t *);
/********************* Application Programming Interface **********************/
void bsp_config(struct setup_t * settings) {
    cli();
    GPIOconfig();
    TimersConfig(settings);
    sei();
}
/****************************** Private  functions ****************************/
static void GPIOconfig(void) {
    DDRB  = 0xff;
    PORTB = 0x0;
    DDRC  = 0x30;
    PORTC = 0x0f;
    DDRD  = 0xff;
    PORTD = 0x0;
}
/*----------------------------------------------------------------------------*/
static void TimersConfig(struct setup_t * ocr) {
    OCR2  = 40;
    OCR1A = ocr->ocr1;
    OCR1B = ocr->ocr2;
    TCCR1A= BIT(WGM10)|BIT(COM1A1)|BIT(COM1B1);
    TCCR1B= BIT(WGM12)|BIT(CS10);
}
/*----------------------------------------------------------------------------*/
ISR (TIMER2_COMP_vect) {
    
}
/******************************************************************************/

//LCD_init();
//
//
//if (control&BIT(0)) {			// Checking the loop entry flag
    //control &= ~BIT(0);			/* Reset the flag */
    //if (!(control&BIT(6))) {	// Check the display initialization completion flag
        //cli();
        //LCD_init();				/* Call initialization of the display */
        //sei();
        //continue;
    //}
    //if (!(control&BIT(5))) {	// Check flag change value
        //cli();
        ///* Selecting a function (converting a value or displaying a value) */
        //if (control&BIT(4)) LCD_show();
        //else convert(choice);
        //sei();
        //continue;
    //}
    //if (control&BIT(1)) {		// Check the button processing flag 
        //cli();
        //control &= ~BIT(1);		/* Reset the flag */
        //time = 125;
        //key_scan();
        //key_action();
        //sei();
    //}
//}
//
//ISR (TIMER2_COMP_vect) {
    //register BYTE t_control = control;
    //t_control |= BIT(0);
    //if(!(--time)) t_control |= BIT(1);
    //control = t_control;
//}
