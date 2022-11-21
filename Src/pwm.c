/**
 * @file    pwm.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 */

/******************************* Included files *******************************/
#include "TaskManager.h"
#include "pwm.h"

#include "EEPROM.h"
//#include "Keyboard.h"
/********************************* Definition *********************************/
#define COMMAND     (0)
#define LCD_DATA    (1)
#define BUSY_FLAG   (7)

/* Display Symbols */
#define f_Ya        (10)
#define f_r         (11)
#define f_k         (12)
#define f_o         (13)
#define f_s         (14)
#define f_t         (15)
#define f_mz        (16)
#define f_K         (17)
#define f_a         (18)
#define f_n         (19)
#define f_l         (20)
#define f_percent   (21)
#define f_number    (22)
#define f_space     (23)
#define f_Sh        (24)
#define f_I         (25)
#define f_M         (26)

#define CHECK_BUSY_FLAG(answer)   {if ((answer) & BIT(BUSY_FLAG)) return; }

enum descriptors_t {
    FORBIDDEN_DESCRIPTOR = 0,
    DISPLAY_INIT,
    DISPLAY_UPD,
    KEYBOARD,
    WATCHDOG,
    MAX_DESCRIPTOR
};
/**************************** Private  variables ******************************/
static BYTE keys_status;
static BYTE font[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                0x39, 0xb1, 0x70, 0xba, 0x6f, 0x63, 0xbf, 0xc4, 0x4b,
                0x61, 0xbd, 0xbb, 0x25, 0xcc, 0x20, 0xac, 0xa5, 0x4d };
static struct symbols_t {
    BYTE channel;
    BYTE hundreds;
    BYTE tens;
    BYTE units;
} symbols = { .channel = 1, .hundreds = 0, .tens = 0, .units = 0 };
/************************ Private  functions prototypes ***********************/
static void lcd_init (void *);
static void scan_keyboard(void *);
static void lcd_write (BYTE, BYTE);
static void convert (struct symbols_t*, BYTE);
static void update_lcd (void*);
/********************* Application Programming Interface **********************/
void pwm_init(struct setup_t * settings) {
    keys_status    = 0;
    BYTE temp      = eeprom_read(0x0000);
    settings->ocr2 = eeprom_read(0x0001);
    settings->ocr1 = temp;

    convert(&symbols, temp);
 }
/*----------------------------------------------------------------------------*/
void start_pwm(void) {
    bsp_speed_up_systime(); // sys tick in 50us
    tm_add_task_with_start_delay(
        DISPLAY_INIT, &lcd_init, NULLPTR, PERIODIC_MODE, 1, 800
    ); //50 us, 40ms delay
    //tm_add_task(WATCHDOG, &wd_throw_bone, NULLPTR, PERIODIC_MODE, 10);
    bsp_start_pwm();
}
/****************************** Private  functions ****************************/
static void finalize_starting(void) {
    bsp_normalize_systime();  // sys tick in 1ms
    tm_kill_task(DISPLAY_INIT);
    tm_add_task(DISPLAY_UPD, &update_lcd, &symbols, PERIODIC_MODE, 1);
    tm_add_task(KEYBOARD, &scan_keyboard, &keys_status, PERIODIC_MODE, 100);
}
/*----------------------------------------------------------------------------*/
static void lcd_write (register BYTE dat_com, register BYTE data) {
    bsp_lcd_write(dat_com, data & 0xf0);
    bsp_lcd_write(dat_com, (data & 0x0f) << 4);
}
/*----------------------------------------------------------------------------*/
static void lcd_init (void * ptr) {
    static BYTE init_stage = 0;
    CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
    switch (init_stage++) {
        case 0: /* setting parameters */
            bsp_lcd_write(COMMAND, 0b00110000);
            break;
        case 1 ... 2: /* setting parameters */
            bsp_lcd_write(COMMAND, 0b00100000);
            bsp_lcd_write(COMMAND, 0b11000000); // two lines, 5 * 11
            break;
        case 3: /* turn on the display */
            bsp_lcd_write(COMMAND, 0b00000000);
            bsp_lcd_write(COMMAND, 0b11000000); // Display on, cursor off, blink off
            break;
        case 4: /* clear the display */
            bsp_lcd_write(COMMAND, 0b00000000);
            bsp_lcd_write(COMMAND, 0b00010000);
            break;
        case 5: /* setting the operating mode */
            bsp_lcd_write(COMMAND, 0b00000000);
            bsp_lcd_write(COMMAND, 0b01100000); // right direction of movement to the cursor, no shift
            break;
        /* Display the initial values of the indicator */
        case 6:
            lcd_write(LCD_DATA,font[f_Sh]);
            break;
        case 7:
            lcd_write(LCD_DATA,font[f_I]);
            break;
        case 8:
            lcd_write(LCD_DATA,font[f_M]);
            break;
        case 9:
            lcd_write(COMMAND,0b10000111);
            break;
        case 10:
            lcd_write(LCD_DATA,font[f_percent]);
            break;
        case 11:
            lcd_write(COMMAND,0b11000000);
            break;
        case 12:
            lcd_write(LCD_DATA,font[f_K]);
            break;
        case 13:
            lcd_write(LCD_DATA,font[f_a]);
            break;
        case 14:
            lcd_write(LCD_DATA,font[f_n]);
            break;
        case 15:
            lcd_write(LCD_DATA,font[f_a]);
            break;
        case 16:
            lcd_write(LCD_DATA,font[f_l]);
            break;
        case 17:
            lcd_write(LCD_DATA,font[f_space]);
            break;
        case 18:
            lcd_write(LCD_DATA,font[f_number]);
            break;
        case 19:
            lcd_write(COMMAND,0b10000100);
            finalize_starting();
            break;
        default:
            init_stage = 20;
            break;
    }
}
/*----------------------------------------------------------------------------*/
static void update_lcd (void * ptr) {
    struct symbols_t * src = (struct symbols_t *)ptr;
    static BYTE counter = 0;
    CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
    switch (counter++) {
        case 0:
            if(src->hundreds) {
                lcd_write(LCD_DATA, font[1]);
            } else {
                lcd_write(LCD_DATA,font[f_space]);
            }
            break;
        case 1:
            if(src->hundreds || src->tens) {
                lcd_write(LCD_DATA, font[src->tens]);
            } else {
                lcd_write(LCD_DATA,font[f_space]);
            }
            break;
        case 2:
            lcd_write(LCD_DATA, font[src->units]);
            break;
        case 3:
            lcd_write(COMMAND, 0b11000111);
            break;
        case 4:
            lcd_write(LCD_DATA,font[src->channel]);
            break;
        case 5:
            lcd_write(COMMAND, 0b10000100);
            tm_kill_task(DISPLAY_UPD);
            counter = 0;
            break;
    }
}
/*----------------------------------------------------------------------------*/
static void convert (struct symbols_t * dst, register BYTE value) {
    register WORD temp1 = value * 100;
    temp1 /= 255;
    if (temp1 == 100) {
        dst->hundreds = 1;
        dst->tens     = 0;
        dst->units    = 0;
    }
    else {
        register BYTE temp2 = 0;
        dst->hundreds = 0;
        while (temp1 >= 10) {
            temp1-=10;
            temp2++;
        }
        dst->tens  = temp2;
        dst->units = temp1;
    }
}
/*----------------------------------------------------------------------------*/
 static void scan_keyboard(void * ptr) {

 }
/******************************************************************************/

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
