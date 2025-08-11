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

int sched_add_task(sched_t *sched, task_fn fn, void *data, uint32_t interval_ms) {
    if (!sched || !fn || sched->tasks_count >= sched->max_tasks) {
        perror("sched_add_task");
        return -1;
    }
    sched_task_t *task = &sched->tasks[sched->tasks_count++];
    task->callback = fn;
    task->data = data;
    task->interval_ms = interval_ms;
    task->last_run_ms = current_time_ms();
    return 0;
}

void sched_start(sched_t *sched) {
    sched->running = 1;
    while (!sched_should_exit()) {
        uint32_t now_ms = current_time_ms();
        for (size_t i = 0; i < sched->tasks_count; i++) {
            sched_task_t *task = &sched->tasks[i];
            if (now_ms - task->last_run_ms >= task->interval_ms) {
                task->callback(task->data);
                task->last_run_ms = now_ms;
            }
        }
        usleep(1000); /* Sleep 1ms to reduce CPU usage. */
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
