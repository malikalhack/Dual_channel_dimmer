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
#define RS      (0)
#define RW      (1)
#define E       (2)
#define DB4     (4)
#define DB5     (5)
#define DB6     (6)
#define DB7     (7)
#define IB      (0)

#define LCD_DATA_MASK       (BIT(DB4) + BIT(DB5) + BIT(DB6) + BIT(DB7))
#define LCD_CONTROL_MASK    (BIT(RS) + BIT(RW) + BIT(E))

#define SYS_TIME_VALUE      (250)
/************************ Private  functions prototypes ***********************/
ISR (TIMER2_COMP_vect);
static void gpio_config(void);
static void timers_config(struct setup_t *);
/**************************** Private  variables ******************************/
static volatile WORD * SysTime;
/********************* Application Programming Interface **********************/
void bsp_config(struct setup_t * settings, volatile WORD * systime) {
    cli();
    SysTime = systime;
    gpio_config();
    timers_config(settings);
    sei();
}
/*----------------------------------------------------------------------------*/
void bsp_start_pwm(void) {
    TCCR1B |= BIT(CS10);
}
/*----------------------------------------------------------------------------*/
void bsp_stop_pwm(void) {
    TCCR1B &= ~BIT(CS10);
}
/*----------------------------------------------------------------------------*/
void bsp_change_pwm(register BYTE channel, register BYTE value) {

}
/*----------------------------------------------------------------------------*/
void bsp_lcd_write (register BYTE dat_com, register BYTE data) {
    PORTD &= ~(LCD_DATA_MASK + LCD_CONTROL_MASK);
    if (dat_com) PORTD|=BIT(RS);
    else PORTD &= ~BIT(RS);

    PORTD |= (data & 0xf0);
    PORTD|=BIT(E);
    asm("nop");
    asm("nop");
    PORTD &= ~BIT(E);
    asm("nop");
    asm("nop");
}
/*----------------------------------------------------------------------------*/
BYTE bsp_lcd_read(register BYTE dat_com) {
    register BYTE data = 0,temp = 0;
    DDRD  = 0b00001111;
    PORTD = 0b11110010;
    if (dat_com) PORTD |= BIT(RS);
    else PORTD &= ~BIT(RS);
    PORTD |= BIT(E);
    asm("nop");
    asm("nop");
    temp = PIND;
    PORTD &= ~BIT(E);
    data = (temp & 0xf0);
    asm("nop");
    asm("nop");
    PORTD |= BIT(E);
    asm("nop");
    asm("nop");
    temp = PIND;
    PORTD &= ~BIT(E);
    temp >>= 4;
    data |= temp;
    asm("nop");
    asm("nop");
    PORTD = 0;
    DDRD = 0b11111111;
    return data;
}
/*----------------------------------------------------------------------------*/
void bsp_speed_up_systime(void) {
    TCCR2 = 0;
    TCNT2 = 0;
    OCR2  = 50;
    TIFR &= BIT(OCF2);
    TCCR2 = BIT(WGM21) + BIT(CS21);
}
/*----------------------------------------------------------------------------*/
void bsp_normalize_systime(void) {
    TCCR2 = 0;
    TCNT2 = 0;
    TIFR &= BIT(OCF2);
    OCR2  = SYS_TIME_VALUE;
    TCCR2 = BIT(WGM21) + BIT(CS21) + BIT(CS20);
}
/****************************** Private  functions ****************************/
static void gpio_config(void) {
    DDRB  = 0xff;
    PORTB = 0x0;
    DDRC  = 0x30;
    PORTC = 0x0f;
    DDRD  = 0xff;
    PORTD = 0x0;
}
/*----------------------------------------------------------------------------*/
static void timers_config(struct setup_t * ocr) {
    OCR2   = SYS_TIME_VALUE;
    OCR1A  = ocr->ocr1;
    OCR1B  = ocr->ocr2;
    TCCR1A = BIT(WGM10)|BIT(COM1A1)|BIT(COM1B1);
    TCCR1B = BIT(WGM12);
    TCCR2  = BIT(WGM21) + BIT(CS21) + BIT(CS20);
    TIMSK  = BIT(OCIE2);
}
/*----------------------------------------------------------------------------*/
ISR (TIMER2_COMP_vect) { (*SysTime)++; } 
/******************************************************************************/
