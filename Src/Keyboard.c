/**
 * @file    keyboard.c
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    27.07.2019 19:12:12
 * @date    21.11.2022
 */

/******************************** Included files ********************************/
#include "keyboard.h"
#include "bsp.h"
/**************************** Private prototypes ******************************/
/**
 * @brief A function that saves a snapshot of the keys to the buffer.
 * @param[in] data Snapshot of keys for buffer.
 */
void set_key (BYTE);
/******************************** Definition **********************************/
#define THRESHOLD       (2)                 /* parameter in s */
#define BUFF_SIZE       (8)

#define COEFFICIENT     (BYTE)(100 / SCAN_KEY_PERIOD)
#define REC_PERIOD      (COEFFICIENT * 4)
#define ACL_REC_PERIOD  (COEFFICIENT * 2)
#define CALC_THRESHOLD  (THRESHOLD * COEFFICIENT * 2)

enum {
    K_STATE_1,
    K_STATE_2,
    K_STATE_3
};
/**************************** Private  variables ******************************/
static BYTE key_stages;
static BYTE prev_stored_pins;
static BYTE duration;

static struct key_buf_t {
    BYTE keys[BUFF_SIZE];
    BYTE head;
    BYTE tail;
    BYTE count;
} key_buf;
/************************** Task Manager functions ****************************/
/** @fn key_scan */
void key_scan (void * pntr) {
    register BYTE current_pins = bsp_get_pins();
    if (!current_pins) {
        key_stages = K_STATE_1;
        return;
    }
    switch (key_stages) {
        case K_STATE_1:
        duration = 0;
        prev_stored_pins = current_pins;
        key_stages = K_STATE_2;
        break;
        case K_STATE_2:
        if (prev_stored_pins == current_pins) {
            set_key(current_pins);
            key_stages = K_STATE_3;
        } else prev_stored_pins = current_pins;
        break;
        case K_STATE_3: {
            if (prev_stored_pins == current_pins) {
                duration = (duration < 205) ? duration + 1 : 200;
                register BYTE div = (duration < CALC_THRESHOLD) ? REC_PERIOD : ACL_REC_PERIOD;

                if (!(duration % div)) set_key(prev_stored_pins);
            }
            else {
                prev_stored_pins = current_pins;
                duration = 0;
                key_stages = K_STATE_2;
            }
            break;
        }
        default: key_stages = K_STATE_1;
    }
}
/************************************** API ***********************************/

/** @fn key_init */
void key_init (void) {
    key_buf.head = 0;
    key_buf.tail = 0;
    key_buf.count = 0;

    key_stages  = K_STATE_1;
    prev_stored_pins = 0;
    duration = 0;
}
/******************************************************************************/

/** @fn get_key */
BYTE key_get (void) {
    register BYTE result = 0;
    if (key_buf.count) {
        result = *(key_buf.keys + key_buf.head);
        if (--key_buf.count) {
            key_buf.head++;
            key_buf.head %= BUFF_SIZE;
        }
    }
    return result;
}
/******************************************************************************/

/** @fn key_check */
BYTE key_check (void) { return key_buf.count; }
/****************************** Private functions *****************************/

/** @fn set_key */
void set_key (register BYTE data) {
    if (key_buf.count) {
        BYTE tail_old = key_buf.tail++;
        key_buf.tail %= BUFF_SIZE;
        if (key_buf.tail == key_buf.head) {
            key_buf.tail = tail_old;
            return; /* The buffer is overloaded */
        }
    }
    key_buf.count++;
    *(key_buf.keys + key_buf.tail) = data;
}
/******************************************************************************/
