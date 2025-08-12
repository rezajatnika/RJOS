#include "scheduler.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t shutdown_requested = 0;

static void handle_signal(int sig) {
    shutdown_requested = 1;
}

/**
 * @brief Gets the current time in millisecond.
 * @return Current time in milliseconds.
 */
static uint32_t current_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

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

int sched_add_task(sched_t *sched, task_fn fn, void *data, uint32_t interval_ms, uint8_t priority) {
    if (!sched || !fn || sched->tasks_count >= sched->max_tasks) {
        perror("sched_add_task");
        return -1;
    }
    sched_task_t *task = &sched->tasks[sched->tasks_count++];
    task->callback = fn;
    task->data = data;
    task->name = "Task";
    task->interval_ms = interval_ms;
    task->last_run_ms = current_time_ms();
    task->priority = priority;
    return 0;
}

void sched_start(sched_t *sched) {
    sched->running = 1;
    sort_tasks_by_priority(sched);

    while (!sched_should_exit()) {
        uint32_t now_ms = current_time_ms();
        uint32_t next_due_ms = UINT32_MAX;

        for (size_t i = 0; i < sched->tasks_count; i++) {
            sched_task_t *task = &sched->tasks[i];

            uint32_t elapsed_ms = now_ms - task->last_run_ms;
            if (elapsed_ms >= task->interval_ms) {
                task->callback(task->data);
                task->last_run_ms = now_ms;

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
        /* Sleep until next task is due, or default to 1ms. */
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

uint32_t millis(void) {
    static uint32_t start_ms = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t now_ms = (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    if (start_ms == 0) {
        start_ms = now_ms;
    }
    return now_ms - start_ms;
}

int sched_should_exit(void) {
    return shutdown_requested;
}

void sched_setup_signal_handlers(void) {
    struct sigaction sa = {
        .sa_handler = handle_signal,
        .sa_flags = 0,
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}
