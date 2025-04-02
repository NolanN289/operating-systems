/**
 * Round-robin scheduling
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

#define TIME_SLICE 10
/*
 * Your code and design here:
 */

struct node* task_list = NULL;

void add(char *name, int priority, int burst) {
    // Create a new task and add it to the end of the task list
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&task_list, newTask);
}

Task* pickNextTask() {
    if (task_list == NULL) {
        return NULL; // No more tasks to pick
    }
    
    Task *nextTask = task_list->task;
    return nextTask;

}

void schedule() {
     Task *currentTask;
    
    // Continue scheduling tasks until all are finished
    while ((currentTask = pickNextTask()) != NULL) {
        if (currentTask->burst > 0) {
            // Determine the time slice, ensuring it does not exceed the remaining burst time
            int timeToRun = (currentTask->burst > TIME_SLICE) ? TIME_SLICE : currentTask->burst;
            
            // Run the task and print its details
            run(currentTask, timeToRun);  // run() already handles the printing and execution

            // Reduce the remaining burst time
            currentTask->burst -= timeToRun;

            // If the task is not finished, put it back at the end of the queue
            if (currentTask->burst > 0) {
                struct node *temp = task_list;
                task_list = task_list->next;
                insert(&task_list, temp->task);
                free(temp);
            } else {
                // Task is completed, no need to re-add it to the queue
                struct node *temp = task_list;
                task_list = task_list->next;
                free(temp);
            }
        }
    }
}