#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "scheduler_pt.h"
#include "system.h"
#include "logger.h"

/**
 * @brief Indicates if a shutdown has been requested.
 * @details This variable is set to 1 when a termination signal
 * is received, signaling the application to begin the shutdown process.
 * It remains 0 otherwise.
 */
static volatile sig_atomic_t shutdown_requested = 0;

static void handle_signal(int sig) {
    (void)sig;
    shutdown_requested = 1;
}

/**
 * @brief Synchronization mutex for scheduler operations.
 * @details Ensures thread-safe access to shared resources within the scheduler
 * by serializing operations that require mutual exclusion.
 */
static pthread_mutex_t sched_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Comparison function for sorting scheduler tasks by priority.
 * @details Compares two scheduler task objects by their priority values.
 * The function returns:
 * - A negative value if the priority of the first task is less than the second.
 * - A positive value if the priority of the first task is greater than the second.
 * - Zero if both tasks have the same priority.
 *
 * This function is designed to be utilized with sorting algorithms such as qsort.
 *
 * @param a Pointer to the first sched_task_t object.
 * @param b Pointer to the second sched_task_t object.
 * @return Integer representing the relative priority of the two tasks.
 */
static int sched_task_cmp(const void *a, const void *b) {
    const sched_task_t *ta = (const sched_task_t *)a;
    const sched_task_t *tb = (const sched_task_t *)b;
    if (ta->priority < tb->priority) return -1;
    if (ta->priority > tb->priority) return  1;
    return 0;
}

/**
 * @brief Sorts the tasks in the scheduler by priority.
 * @details This function organizes the tasks in the scheduler structure in
 * descending order of priority, ensuring that higher-priority tasks are scheduled first.
 *
 * @param sched Pointer to the scheduler structure that contains the task list to be sorted.
 */
static void sort_tasks_by_priority(sched_t *sched) {
    qsort(sched->tasks, sched->tasks_count, sizeof(sched_task_t), sched_task_cmp);
}

static void *sched_task_thread(void *arg) {
    sched_ctx_t *ctx = (sched_ctx_t *)arg;

    /* Lock for status update. */
    sched_task_t *task = ctx->task;
    pthread_mutex_lock(&sched_mutex);
    task->deadline_ms = task->last_run_ms + task->interval_ms;
    pthread_mutex_unlock(&sched_mutex);

    uint32_t start = millis();
    task->callback(task->data);
    uint32_t duration = millis() - start;

    uint32_t now_ms = ctx->now_ms;
    pthread_mutex_lock(&sched_mutex);
    task->last_run_ms = now_ms;
    task->run_count++;
    task->total_duration_ms += duration;
    if (duration > task->max_duration_ms) {
        task->max_duration_ms = duration;
    }
    if (millis() > task->deadline_ms) {
        task->overrun_count++;
        logger_log(LOG_LEVEL_INFO, "Task %s exceeded deadline by %ums.", millis() - task->deadline_ms);
    }
    sched_t *sched = ctx->sched;
    size_t idx = ctx->idx;
    if (sched->log_hook) {
        sched->log_hook(idx, task->data);
    }
    pthread_mutex_unlock(&sched_mutex);

    free(ctx);
    return NULL;
}

int sched_init(sched_t *sched, size_t max_tasks) {
    sched->tasks = calloc(max_tasks, sizeof(sched_task_t));
    if (!sched->tasks) {
        return -1;
    }
    sched->max_tasks = max_tasks;
    sched->tasks_count = 0;
    sched->running = 0;
    sched->log_hook = NULL;
    return 0;
}

int sched_add_task(sched_t *sched, task_fn fn, void *data, uint32_t interval_ms, uint8_t priority, const char *name) {
    if (!sched || !fn || sched->tasks_count >= sched->max_tasks) {
        return -1;
    }
    sched_task_t *task = &sched->tasks[sched->tasks_count++];
    task->callback = fn;
    task->data = data;
    task->name = strdup(name);
    task->interval_ms = interval_ms;
    task->last_run_ms = millis();
    task->priority = priority;
    task->run_count = 0;
    task->total_duration_ms = 0;
    task->max_duration_ms = 0;
    task->overrun_count = 0;
    return 0;
}

void sched_start(sched_t *sched) {
    sched->running = 1;
    sort_tasks_by_priority(sched);

    pthread_t *worker_threads = calloc(sched->tasks_count, sizeof(pthread_t));
    if (!worker_threads) {
        return;
    }

    while (!sched_should_exit()) {
        uint32_t now_ms      = millis();
        uint32_t next_due_ms = UINT32_MAX;

        size_t thread_count = 0;
        for (size_t i = 0; i < sched->tasks_count; i++) {
            sched_task_t *task = &sched->tasks[i];
            pthread_mutex_lock(&sched_mutex);
            uint32_t elapsed_ms = now_ms - task->last_run_ms;
            pthread_mutex_unlock(&sched_mutex);

            if (elapsed_ms >= task->interval_ms) {
                /* Prepare thread args. */
                sched_ctx_t *ctx = malloc(sizeof(sched_ctx_t));
                ctx->sched  = sched;
                ctx->task   = task;
                ctx->now_ms = now_ms;
                ctx->idx    = i;

                /* Start the thread. */
                if (pthread_create(&worker_threads[thread_count++], NULL, sched_task_thread, ctx) != 0) {
                    free(ctx);
                    continue;
                }
            } else {
                uint32_t diff_ms = task->interval_ms - elapsed_ms;
                if (diff_ms < next_due_ms) {
                    next_due_ms = diff_ms;
                }
            }
        }
        /* Wait for all threads to finish before the next scheduler ticks. */
        for (size_t i = 0; i < thread_count; i++) {
            pthread_join(worker_threads[i], NULL);
        }
        uint32_t usleep_mul = (next_due_ms == UINT32_MAX) ? 1 : next_due_ms;
        usleep(usleep_mul * 1);
    }
    free(worker_threads);
}

void sched_stop(sched_t *sched) {
    sched->running = 0;
}

void sched_destroy(sched_t *sched) {
    for (size_t i = 0; i < sched->tasks_count; ++i) {
        free(sched->tasks[i].name);
    }
    free(sched->tasks);
    sched->tasks = NULL;
    sched->max_tasks   = 0;
    sched->tasks_count = 0;
}

void sched_set_log_hook(sched_t *sched, sched_log_fn log_hook) {
    if (sched) {
        sched->log_hook = log_hook;
    }
}

int sched_should_exit(void) {
    return shutdown_requested != 0;
}

void sched_setup_signal_handlers(void) {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    (void)sigaction(SIGINT,  &sa, NULL);
    (void)sigaction(SIGTERM, &sa, NULL);
}