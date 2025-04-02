/*
 * Priority scheduling
 */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

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

    // Initialize the highest priority task as the first one
    struct node *current = task_list;
    struct node *maxPriorityNode = task_list;
    
    // Traverse the list to find the task with the highest priority
    while (current != NULL) {
        if (current->task->priority > maxPriorityNode->task->priority) {
            maxPriorityNode = current; // Update the task with the highest priority
        }
        current = current->next; // Move to the next node
    }

    // After finding the node with the highest priority, return the task
    Task *nextTask = maxPriorityNode->task;

    // Remove the highest priority node from the list
    if (maxPriorityNode == task_list) {
        // If the highest priority task is the first node
        task_list = task_list->next;
    } else {
        // Traverse again to find the node before the maxPriorityNode
        current = task_list;
        while (current->next != maxPriorityNode) {
            current = current->next;
        }
        current->next = maxPriorityNode->next; // Skip the maxPriorityNode
    }

    free(maxPriorityNode); // Free the memory of the removed node

    return nextTask;


}

void schedule() {
    Task *currentTask;
    while ((currentTask = pickNextTask()) != NULL) {
        // Run the task returned by pickNextTask
        run(currentTask, currentTask->burst);
    }
}