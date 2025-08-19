#include "sched_util.h"
#include "scheduler.h"

#include <stdlib.h>

/**
 * @brief Comparison function for sorting scheduler tasks by priority.
 * @details Compares two scheduler task objects by their priority values.
 * The function returns:
 * - A negative value if the priority of the first task is less than the second.
 * - A positive value if the priority of the first task is greater than the second.
 * - Zero if both tasks have the same priority.
 *
 * This function is designed to be used with sorting algorithms such as qsort.
 *
 * @param a Pointer to the first sched_task_t object.
 * @param b Pointer to the second sched_task_t object.
 * @return Integer representing the relative priority of the two tasks.
 */
int sched_task_cmp(const void *a, const void *b) {
    const sched_task_t *ta = a;
    const sched_task_t *tb = b;
    if (ta->priority < tb->priority) return -1;
    if (ta->priority > tb->priority) return  1;
    return 0;
}

void sort_tasks_by_priority(const void *sched) {
    const sched_t *s = sched;
    qsort(s->tasks, s->tasks_count, sizeof(sched_task_t), sched_task_cmp);
}
