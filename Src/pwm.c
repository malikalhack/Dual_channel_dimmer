/**
 * @file    pwm.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    19.11.2022
 * @date    21.11.2022
 */

/******************************* Included files *******************************/
#include "TaskManager.h"
#include "pwm.h"
#include "wdt.h"
#include "eeprom.h"
#include "keyboard.h"
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

#define EEPROM_ADDR_OCR1    (0x0005)
#define EEPROM_ADDR_OCR2    (0x0006)
#define EVERY_SYS_TICK      (1)
#define PWM_CTRL_PERIOD     (500)
#define WATCH_DOG_PERIOD    (100)

enum descriptors_t {
    FORBIDDEN_DESCRIPTOR = 0,
    DISPLAY_INIT,
    DISPLAY_UPD,
    KEYBOARD,
    WATCHDOG,
    PWM,
    MAX_DESCRIPTOR
};
/**************************** Private  variables ******************************/
static const BYTE font[] = {
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0xb1, 0x70, 0xba, 0x6f, 0x63, 0xbf, 0xc4, 0x4b,
    0x61, 0xbd, 0xbb, 0x25, 0xcc, 0x20, 0xac, 0xa5, 0x4d
};
static struct symbols_t {
    BYTE channel;
    BYTE hundreds;
    BYTE tens;
    BYTE units;
} symbols = { .channel = 1, .hundreds = 0, .tens = 0, .units = 0 };
/************************ Private  functions prototypes ***********************/
static void lcd_init (void *);
static void lcd_write (BYTE, BYTE);
static void convert (struct symbols_t*, BYTE);
static void update_lcd (void*);
static void pwm_control (void*);
/********************* Application Programming Interface **********************/
void pwm_init(struct setup_t * settings) {
    BYTE temp      = eeprom_read(EEPROM_ADDR_OCR1);
    settings->ocr2 = eeprom_read(EEPROM_ADDR_OCR2);
    settings->ocr1 = temp;

    convert(&symbols, temp);
 }
/*----------------------------------------------------------------------------*/
void start_pwm(void) {
    bsp_speed_up_systime(); // sys tick in 50us
    tm_add_task_with_start_delay(
        DISPLAY_INIT, &lcd_init, NULLPTR, PERIODIC_MODE, EVERY_SYS_TICK, 800
    ); //50 us, 40ms delay
    bsp_start_pwm();
}
/****************************** Private  functions ****************************/
static void finalize_starting(void) {
    bsp_normalize_systime();  // sys tick in 1ms
    tm_kill_task(DISPLAY_INIT);
    tm_add_task(DISPLAY_UPD, &update_lcd, &symbols, PERIODIC_MODE, EVERY_SYS_TICK);
    tm_add_task(KEYBOARD, &key_scan, NULLPTR, PERIODIC_MODE, SCAN_KEY_PERIOD);
    tm_add_task(PWM, &pwm_control, &symbols, PERIODIC_MODE, PWM_CTRL_PERIOD);
    tm_add_task(WATCHDOG, &wd_throw_bone, NULLPTR, PERIODIC_MODE, WATCH_DOG_PERIOD);
    wdt_start();
}
/*----------------------------------------------------------------------------*/
static void lcd_write (register BYTE dat_com, register BYTE data) {
    bsp_lcd_write(dat_com, data & 0xf0);
    bsp_lcd_write(dat_com, (data & 0x0f) << 4);
}
/*----------------------------------------------------------------------------*/
static void lcd_init (void * ptr) {
    static BYTE init_stage = 0;
    static const BYTE init_data[] = {
        0b10101100, 0b10100101, 0b01001101, 0b10000111, 0b00100101,
        0b11000000, 0b01001011, 0b01100001, 0b10111101, 0b01100001,
        0b10111011, 0b00100000, 0b11001100
    };
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
        case 6 ... 18: {
            register BYTE command;
            if (init_stage == 10 || init_stage == 12) { command = COMMAND; }
            else { command = LCD_DATA; }
            lcd_write(command, init_data[init_stage - 7]);
            break;
            }
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
                lcd_write(LCD_DATA, font[f_space]);
            }
            break;
        case 1:
            if(src->hundreds || src->tens) {
                lcd_write(LCD_DATA, font[src->tens]);
            } else {
                lcd_write(LCD_DATA, font[f_space]);
            }
            break;
        case 2:
            lcd_write(LCD_DATA, font[src->units]);
            break;
        case 3:
            lcd_write(COMMAND, 0b11000111);
            break;
        case 4:
            lcd_write(LCD_DATA, font[src->channel]);
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
static void pwm_control(void * ptr) {
    struct symbols_t * src = (struct symbols_t *)ptr;
    register BOOLEAN visual_changes = FALSE;
    register BOOLEAN pwm_changes = FALSE;
    register BYTE iter = key_check();
    while (iter--) {
        register BYTE curr_ocr =
            (src->channel == 1) ? bsp_get_ocr1() : bsp_get_ocr2();
        switch (key_get()) {
            case BIT(BTN_UP):
                if (curr_ocr < 255) {
                    bsp_change_pwm(src->channel, ++curr_ocr);
                    visual_changes = TRUE;
                    pwm_changes = TRUE;
                }
                break;
            case BIT(BTN_DOWN):
                if (curr_ocr > 0) {
                    bsp_change_pwm(src->channel, --curr_ocr);
                    visual_changes = TRUE;
                    pwm_changes = TRUE;
                }
                break;
            case BIT(BTN_LEFT):
                if (src->channel != 1) {
                    src->channel = 1;
                    curr_ocr = bsp_get_ocr1();
                    visual_changes = TRUE;
                }
                break;
            case BIT(BTN_RIGHT):
                if (src->channel != 2) {
                    src->channel = 2;
                    curr_ocr = bsp_get_ocr2();
                    visual_changes = TRUE;
                }
                break;
            default: break;
        }
        if (visual_changes) {
            convert(src, curr_ocr);
            tm_add_task(DISPLAY_UPD, &update_lcd, src, PERIODIC_MODE, 1);
        }
    }
    if(pwm_changes) {
        eeprom_write(EEPROM_ADDR_OCR1, bsp_get_ocr1());
        eeprom_write(EEPROM_ADDR_OCR2, bsp_get_ocr2());
    }
}
/******************************************************************************/
