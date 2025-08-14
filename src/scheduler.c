#include "scheduler.h"
#include "system.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Indicates if a shutdown has been requested.
 * @details This variable is set to 1 when a termination signal
 * is received, signaling the application to begin the shutdown process.
 * It remains 0 otherwise.
 */
static volatile sig_atomic_t shutdown_requested = 0;

/**
 * @brief Handles incoming signals and triggers shutdown.
 * @details This function is invoked when a registered signal is received.
 * It sets the shutdown_requested flag to indicate that the shutdown process
 * should begin.
 * @param sig The signal number received by the application.
 */
static void handle_signal(int sig) {
    shutdown_requested = 1;
}

/**
 * @brief Sorts tasks in the scheduler by priority in descending order.
 * @details The function rearranges the tasks in the scheduler such that
 * tasks with higher priority values appear earlier in the array.
 * It uses an insertion sort algorithm.
 *
 * @param sched Pointer to the scheduler containing the tasks to be sorted.
 */
static void sort_tasks_by_priority(sched_t *sched) {
    for (size_t i = 0; i < sched->tasks_count; ++i) {
        sched_task_t key = sched->tasks[i];
        size_t j = i;
        while (j > 0 && sched->tasks[j-1].priority < key.priority) {
            sched->tasks[j] = sched->tasks[j-1];
            --j;
        }
        sched->tasks[j] = key;
    }
}

int sched_init(sched_t *sched, size_t max_tasks) {
    sched->tasks = calloc(max_tasks, sizeof(sched_task_t));
    if (!sched->tasks) {
        perror("sched_init");
        return -1;
    }
    sched->max_tasks = max_tasks;
    sched->tasks_count = 0;
    sched->running = 0;
    return 0;
}

int sched_add_task(sched_t *sched, task_fn fn, void *data, uint32_t interval_ms, uint8_t priority, char *name) {
    if (!sched || !fn || sched->tasks_count >= sched->max_tasks) {
        perror("sched_add_task");
        return -1;
    }
    sched_task_t *task = &sched->tasks[sched->tasks_count++];
    task->callback = fn;
    task->data = data;
    task->name = name;
    task->interval_ms = interval_ms;
    task->last_run_ms = millis();
    task->priority = priority;
    return 0;
}

void sched_start(sched_t *sched) {
    sched->running = 1;
    sort_tasks_by_priority(sched);

    while (!sched_should_exit()) {
        uint32_t now_ms = millis();
        uint32_t next_due_ms = UINT32_MAX;

        for (size_t i = 0; i < sched->tasks_count; i++) {
            sched_task_t *task = &sched->tasks[i];

            uint32_t elapsed_ms = now_ms - task->last_run_ms;
            if (elapsed_ms >= task->interval_ms) {
                task->deadline_ms = task->last_run_ms + task->interval_ms;

                uint32_t start = millis();
                task->callback(task->data);
                uint32_t duration = millis() - start;
                task->last_run_ms = now_ms;

                task->run_count++;
                task->total_duration_ms += duration;
                if (duration > task->max_duration_ms) {
                    task->max_duration_ms = duration;
                }

                /* Overrun detection. */
                if (millis() > task->deadline_ms) {
                    task->overrun_count++;
                    fprintf(stderr, "[Overrun] Task %s exceeded deadline by %ums.\n",
                        task->name, millis() - task->deadline_ms);
                }

                /* Call logging hook if set. */
                if (sched->log_hook) {
                    sched->log_hook(i, task->data);
                }
            } else {
                uint32_t diff_ms = task->interval_ms - elapsed_ms;
                if (diff_ms < next_due_ms) {
                    next_due_ms = diff_ms;
                }
            }
        }
        /* Sleep until next task is due, or default to 1 ms. */
        uint32_t sleep_ms;
        if (next_due_ms == UINT32_MAX) {
            sleep_ms = 1;
        } else {
            sleep_ms = next_due_ms;
        }
        usleep(sleep_ms * 100);
    }
}

void sched_stop(sched_t *sched) {
    sched->running = 0;
}

void sched_destroy(sched_t *sched) {
    free(sched->tasks);
    sched->tasks = NULL;
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
