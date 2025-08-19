#ifndef SCHED_UTIL_H
#define SCHED_UTIL_H

/**
 * @brief Sorts the tasks in the scheduler by priority.
 * @details This function organizes the tasks in the scheduler structure in
 * descending order of priority, ensuring that higher-priority tasks are scheduled first.
 */
void sort_tasks_by_priority(const void *sched);

#endif
