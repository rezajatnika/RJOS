#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stddef.h>

typedef void (*task_fn)(void *data);
typedef void (*sched_log_fn)(size_t idx, void *data);

/**
 * @brief Represents a single scheduled task.
 */
typedef struct {
    const char *name;
    task_fn callback;       /**< Function to execute */
    void *data;             /**< Arguments of the function to execute */
    uint32_t interval_ms;   /**< Execution interval in milliseconds */
    uint32_t last_run_ms;   /**< Timestamp of last execution */
    uint8_t priority;       /**< 0 = Lowest, 255 = Highest */
} sched_task_t;

/**
 * @brief Scheduler structure holding task list and metadata.
 */
typedef struct {
    sched_task_t *tasks;
    size_t max_tasks;
    size_t tasks_count;
    int running;
    sched_log_fn log_hook;
} sched_t;

/**
 * @brief Initializes the scheduler with a fixed number of task slots.
 *
 * Allocates memory for the task array and prepares the scheduler for use.
 * This must be called before adding tasks or running the scheduler.
 * @param sched Pointer to a scheduler instance to initialize.
 * @param max_tasks Maximum number of tasks the scheduler can manage.
 * @return 0 on success, -1 on memory allocation failure.
 */
int sched_init(sched_t *sched, size_t max_tasks);

/**
 * @brief Adds a task to the scheduler.
 *
 * Registers a function to be executed at a fixed interval.
 * @param sched Pointer to a scheduler.
 * @param fn Function to execute.
 * @param data Arguments of the function to be executed.
 * @param interval_ms Interval in milliseconds between executions.
 * @return 0 on success, -1 if task list if full.
 */
int sched_add_task(sched_t *sched, task_fn fn, void *data, uint32_t interval_ms, uint8_t priority);

/**
 * @brief Starts the scheduler loop once.
 * @param sched Pointer to the scheduler.
 */
void sched_start(sched_t *sched);

/**
 * @brief Stops the scheduler.
 * @param sched Pointer to the scheduler.
 */
void sched_stop(sched_t *sched);

/**
 * @brief Frees resources associated with the scheduler.
 * @param sched Pointer to the scheduler.
 */
void sched_destroy(sched_t *sched);

/**
 *
 * @param sched
 * @param log_hook
 */
void sched_set_log_hook(sched_t *sched, sched_log_fn log_hook);

/**
 * @brief Returns true if a shutdown signal has been received.
 */
int sched_should_exit(void);

/**
 * @brief Installs signal handlers for graceful shutdown.
 */
void sched_setup_signal_handlers(void);

#endif
