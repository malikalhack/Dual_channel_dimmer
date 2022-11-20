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

#define CHECK_BUSY_FLAG(answer)   {if ((answer) & BIT(BUSY_FLAG)) break; }



enum descriptors_t {
    FORBIDDEN_DESCRIPTOR = 0,
    DISPLAY_INIT,
    KEYBOARD,
    WATCHDOG,
    MAX_DESCRIPTOR
};
/**************************** Private  variables ******************************/
static BYTE keys_status;
BYTE font[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                0x39, 0xb1, 0x70, 0xba, 0x6f, 0x63, 0xbf, 0xc4, 0x4b,
                0x61, 0xbd, 0xbb, 0x25, 0xcc, 0x20, 0xac, 0xa5, 0x4d };
/************************ Private  functions prototypes ***********************/
static void lcd_init (void *);
static void scan_keyboard(void *);
static void lcd_write (BYTE, BYTE);
//void lcd_show (void);
//void lcd_convert (register BYTE value);
/********************* Application Programming Interface **********************/
void pwm_init(struct setup_t * settings) {
    keys_status    = 0;
    settings->ocr1 = eeprom_read(0x0000);
    settings->ocr2 = eeprom_read(0x0001);
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
    switch (init_stage) {
        case 0: /* setting parameters */
            bsp_lcd_write(COMMAND, 0b00110000);
            init_stage++;
            break;
        case 1 ... 2: /* setting parameters */
            bsp_lcd_write(COMMAND, 0b00100000);
            bsp_lcd_write(COMMAND, 0b11000000); // two lines, 5 * 11
            init_stage++;
            break;
        case 3: /* turn on the display */
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            bsp_lcd_write(COMMAND, 0b00000000);
            bsp_lcd_write(COMMAND, 0b11000000); // Display on, cursor off, blink off
            init_stage++;
            break;
        case 4: /* clear the display */
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            bsp_lcd_write(COMMAND, 0b00000000);
            bsp_lcd_write(COMMAND, 0b00010000);
            init_stage++;
            break;
        case 5: /* setting the operating mode */
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            bsp_lcd_write(COMMAND, 0b00000000);
            bsp_lcd_write(COMMAND, 0b01100000); // right direction of movement to the cursor, no shift
            init_stage++;
            break;
        /* Display the initial values of the indicator */
        case 6:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_Sh]);
            init_stage++;
            break;
        case 7:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_I]);
            init_stage++;
            break;
        case 8:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_M]);
            init_stage++;
            break;
        case 9:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(COMMAND,0b10000111);
            init_stage++;
            break;
        case 10:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_percent]);
            init_stage++;
            break;
        case 11:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(COMMAND,0b11000000);
            init_stage++;
            break;
        case 12:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_K]);
            init_stage++;
            break;
        case 13:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_a]);
            init_stage++;
            break;
        case 14:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_n]);
            init_stage++;
            break;
        case 15:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_a]);
            init_stage++;
            break;
        case 16:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_l]);
            init_stage++;
            break;
        case 17:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_space]);
            init_stage++;
            break;
        case 18:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(LCD_DATA,font[f_number]);
            init_stage++;
            break;
        case 19:
            CHECK_BUSY_FLAG(bsp_lcd_read(COMMAND));
            lcd_write(COMMAND,0b10000100);
            finalize_starting();
            init_stage++;
            break;
        default: break;
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
