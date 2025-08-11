#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
    while (sched->running) {
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
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
