#ifndef MACROS_H
#define MACROS_H

/**
 * @brief Macro to add a task to the scheduler with simplified syntax
 *
 * This macro provides a convenient way to add tasks to the scheduler without
 * having to explicitly pass all parameters. It wraps the sched_add_task function
 * call and can be used for more readable and concise code.
 *
 * @param func Task function pointer (task_fn type)
 * @param task_data Pointer to data passed to the task function
 * @param interval Task execution interval in milliseconds
 * @param prio Task priority (uint8_t)
 * @param task_name String name for the task
 */
#define SCHED_ADD_TASK(func, task_data, interval, prio, task_name) sched_add_task((func), (task_data), (interval), (prio), (task_name))


#endif