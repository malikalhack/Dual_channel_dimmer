/**
 * @brief Dynamic Task Manager
 * @file    TaskManager.h
 * @version 3.3.0
 * @authors Anton Chernov
 * @date    17.06.2019
 * @date    22.11.2022
 */

#ifndef TASK_MANAGER_H_
#define TASK_MANAGER_H_
/****************************** Included files ********************************/
#include "standard.h"
/******************************** Definition **********************************/

#define INVALID_ID  (0)

/**
 * @enum EModes
 * @brief Defines the task's operating modes.
 */
enum EModes {
    eRealtime = 0,
    eOnetime,
    ePeriodic,
    eLimitedLifetime,
    eStandby,
    eIdle,
    eMaxMode
};

typedef void * param_t;
typedef void task_func_t (param_t);

/****************************** Prototypes API ********************************/

/**
 * @brief The primary task manager function.
 * @details The dispatcher calls functions from the stack one by one in
 * an infinite loop. It does not take or return any arguments.
 */
    int tm_run (void);

/**
 * @brief Resets the task manager.
 * @details It resets the counter and sets the initial values of the stack.
 */
    void tm_reset (void);

/**
 * @brief Initializes the system timer pointer.
 * @param[in] time_ptr - the pointer of the timer variable.
 */
    void tm_init (volatile word_t const * time_ptr);

/**
 * @details Adds a process to the task manager stack.
 * @param[in] dscr - process descriptor
 * @param[in] func - function pointer
 * @param[in] params - function argument pointer
 * @param[in] mode - execution mode
 * @param[in] run_time - time value
 */
    void tm_add_task (
        byte_t dscr,
        task_func_t func,
        param_t params,
        byte_t mode,
        word_t run_time
    );

/**
 * @brief Adds a process to the task manager stack with defined start delay.
 * @param[in] dscr - process descriptor
 * @param[in] func - function pointer
 * @param[in] params - function argument pointer
 * @param[in] mode - execution mode
 * @param[in] run_time - time value
 * @param[in] delay - delay value
 */
    void tm_add_task_with_start_delay (
        byte_t dscr,
        task_func_t func,
        param_t params,
        byte_t mode,
        word_t run_time,
        word_t delay
    );

/**
 * @brief Removes a process from the task manager stack.
 * @param[in] dscr - process descriptor.
 */
    void tm_kill_task (byte_t dscr);

/**
 * @brief Removes a process from the task manager stack.
 * @param[in] id - process ID.
 */
    void tm_kill_task_by_id (byte_t id);

/**
 * @brief Switches the selected process to standby mode for a specified time.
 * @param[in] id - process ID
 * @param[in] next_run_time - time value
 */
    void tm_put_task_on_standby (byte_t id, word_t next_run_time);

/**
 * @brief Switches the selected process to the specified mode.
 * @param[in] id - process ID
 * @param[in] mode - execution mode
 * @param[in] run_time - time value
 */
    void tm_set_task_mode (byte_t id, byte_t mode, word_t run_time);

/**
 * @brief Replaces the selected process with another.
 * @param[in] id - process ID
 * @param[in] func - function pointer
 * @param[in] mode - execution mode
 * @param[in] run_time - time value
 */
    void tm_replace_task (
        byte_t id,
        task_func_t func,
        byte_t mode,
        word_t run_time
    );

/**
 * @brief Returns the process ID for the given descriptor.
 * @param[in] dscr - process descriptor 
 * @Returns process ID
 */
    byte_t tm_get_task_id (byte_t dscr);

/******************************************************************************/
#endif /* !TASK_MANAGER_H_ */
