//priority_queue.h
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

//forwarded declaration of task structure from task_manager.h
typedef struct Task Task;

#define MAX_TASKS 500

//priority queue structure with minheap array-based
typedef struct {
    Task* data[MAX_TASKS];
    int   size;
} PriorityQueue;

void  initQueue(PriorityQueue* pq);
void  bubblein(PriorityQueue* pq, Task* t);
Task* pophigh(PriorityQueue* pq);
Task* peekhigh(PriorityQueue* pq);
int   compareTask(Task* a, Task* b);

#endif
