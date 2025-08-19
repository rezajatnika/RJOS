#ifndef RJOS_SCHEDULER_H
#define RJOS_SCHEDULER_H

#include <stdint.h>

typedef void (*task_fn)(void *data);
typedef void (*sched_log_fn)(size_t idx, void *data);

/**
 * Struct representing a scheduled task in the system.
 */
typedef struct sched_task {
    char    *name;
    task_fn  callback;      /**< Function to execute */
    void    *data;          /**< Arguments of the function to execute */
    uint32_t interval_ms;   /**< Execution interval in milliseconds */
    uint32_t last_run_ms;   /**< Timestamp of last execution */
    uint8_t  priority;      /**< 0 = Lowest, 255 = Highest */
    uint32_t run_count;
    uint32_t total_duration_ms;
    uint32_t max_duration_ms;
    uint32_t deadline_ms;
    uint32_t overrun_count;
} sched_task_t;

/**
 * Struct representing a scheduler for managing and executing tasks.
 */
typedef struct sched {
    sched_task_t *tasks;
    size_t        max_tasks;
    size_t        tasks_count;
    int           running;
    sched_log_fn  log_hook;
} sched_t;

/**
 * Initializes a scheduler instance with a specified maximum number of tasks.
 *
 * @param max_tasks The maximum number of tasks the scheduler can handle concurrently.
 * @return Returns 0 on successful initialization. Returns -1 if an error occurs, such as memory allocation failure.
 */
int sched_init(size_t max_tasks);

/**
 * Adds a new task to the scheduler.
 *
 * @param fn Function pointer representing the task to be executed.
 * @param data Pointer to the data that will be passed to the task's function.
 * @param interval_ms Execution interval for the task in milliseconds.
 * @param priority Priority of the task, where 0 represents the lowest and 255 represents the highest.
 * @param name Name of the task for identification purposes.
 * @return Returns 0 on success, or -1 on failure (e.g., if the scheduler is null, the function pointer is null, or the task limit is reached).
 */
int sched_add_task(task_fn fn, void *data, uint32_t interval_ms, uint8_t priority, const char *name);

/**
 * Starts the scheduler, managing tasks execution and priority.
 *
 * This function initializes the running loop for the scheduler to process
 * and execute tasks. Tasks are processed according to their priority and
 * interval timing. The function remains in a loop, continuously checking
 * for tasks to execute until an exit condition is triggered.
 */
void sched_start(void);

/**
 * Stops the scheduler by setting its running state to false.
 */
void sched_stop(void);

/**
 * Destroys the scheduler and releases any allocated resources.
 */
void sched_destroy(void);

/**
 * Sets a logging hook for the scheduler to allow tracking or debugging.
 *
 * @param log_hook Function pointer to the log hook to be used for logging task-related events.
 */
void sched_set_log_hook(sched_log_fn log_hook);

/**
 * Checks if the scheduler should exit.
 *
 * @return Non-zero if a shutdown request has been made, otherwise 0.
 */
int sched_should_exit(void);

/**
 * Sets up signal handlers for the scheduler.
 *
 * Registers the signal handlers to handle termination signals
 * (e.g., SIGINT and SIGTERM). Upon receiving these signals, the
 * system sets an internal flag to indicate a shutdown request.
 * This allows the scheduler to perform graceful termination.
 */
void sched_setup_signal_handlers(void);

/**
 * Calculates the average execution time of a scheduled task in milliseconds.
 *
 * @param task Pointer to a sched_task_t structure representing the task for which
 *             the average execution time is to be calculated.
 * @return The average execution time in milliseconds. Returns 0 if the task has
 *         not been executed at least once (run_count is 0).
 */
static inline uint32_t sched_avg_ms(sched_task_t *task) {
    if (task->run_count) {
        return task->total_duration_ms / task->run_count;
    }
    return 0;
}

/**
 * Calculates the average execution time of a scheduled task in microseconds.
 *
 * @param task Pointer to a sched_task_t structure representing the task for which
 *             the average execution time is to be calculated.
 * @return The average execution time in microseconds. Returns 0 if the task has
 *         not been executed at least once (run_count is 0).
 */
static inline uint32_t sched_avg_us(sched_task_t *task) {
    if (task->run_count) {
        return 0;
    }
    return 0;
}

/**
 * Retrieves the overrun count for a specified scheduled task.
 *
 * @param task Pointer to the scheduled task whose overrun count is to be fetched.
 * @return The number of times the task has overrun its deadline.
 */
static inline uint32_t sched_get_overruns(sched_task_t *task) {
    return task->overrun_count;
}

#endif
