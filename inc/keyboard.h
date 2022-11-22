/**
 * @file    keyboard.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    27.07.2019 19:12:12
 * @date    21.11.2022
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_
/******************************** Included files ********************************/
#include "standard.h"
/********************************** Definition **********************************/
#define SCAN_KEY_PERIOD    (20)    /* parameter in ms */
/******************** Task Manager functions prototypes ***********************/

/**
 * @brief Task Manager function to scan buttons. 
 * @param[in] pntr void pointer (unused)
 */
void key_scan (void *);
/****************************** Prototypes API ********************************/

/** @brief An initialize function */
void key_init (void);

/**
 * @brief A function that returns a snapshot of the keys from the buffer.
 * @returns Snapshot of keys from the buffer.
 */
BYTE key_get (void);

/**
 * @brief A function that returns the number of key shots in the buffer.
 * @returns The number of key snapshots in the buffer. 
 */
BYTE key_check (void);
/********************************************************************************/
#endif /* KEYBOARD_H_ */
