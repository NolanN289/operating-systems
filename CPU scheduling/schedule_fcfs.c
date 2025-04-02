/**
 * FCFS scheduling
 */
 
#include <stdlib.h>
#include <stdio.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

//Your code and design here

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

    // FCFS picks the first task in the list
    Task *nextTask = task_list->task;

    // Remove the head node and update the list pointer
    struct node *temp = task_list;
    task_list = task_list->next;
    free(temp);

    return nextTask;
}

void schedule() {
    Task *currentTask;
    while ((currentTask = pickNextTask()) != NULL) {
        // Run the task returned by pickNextTask
        run(currentTask, currentTask->burst);
    }
}