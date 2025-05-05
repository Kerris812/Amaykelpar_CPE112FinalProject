//priority_queue.c
#include "priority_queue.h"
#include "task_manager.h"

#include <stdlib.h>

//for index
static int parent(int i){ 
    return (i - 1) / 2; 
}
static int left  (int i){ 
    return 2 * i + 1;     
}
static int right (int i){ 
    return 2 * i + 2;     
}

//compare two tasks first by priority, if priorities are equal compare by date and time
int compareTask(Task* a, Task* b) {
    if (a->priority != b->priority) {
        return a->priority - b->priority;
    }

    //already implement dates compare function in task_manager.c
    return sooner(a->dueDate, a->dueTime, b->dueDate, b->dueTime);
}

static void heapiheapi(PriorityQueue* pq, int i) {
    int l = left(i), r = right(i), smallest = i;
    //check if left child is smaller than current
    if (l < pq->size && compareTask(pq->data[l], pq->data[smallest]) < 0)
        smallest = l;
    //check if right child is even smaller
    if (r < pq->size && compareTask(pq->data[r], pq->data[smallest]) < 0)
        smallest = r;
    if (smallest != i) {
        Task* tmp = pq->data[i];
        pq->data[i] = pq->data[smallest];
        pq->data[smallest] = tmp;
        heapiheapi(pq, smallest);
    }
}

//initialize
void initQueue(PriorityQueue* pq) {
    pq->size = 0;
}

void bubblein(PriorityQueue* pq, Task* t) {
    if (pq->size >= MAX_TASKS) return;
    int i = pq->size++;
    pq->data[i] = t;

    //bubble sort
    while (i && compareTask(pq->data[i], pq->data[parent(i)]) < 0) {
        Task* tmp = pq->data[i];
        pq->data[i] = pq->data[parent(i)];
        pq->data[parent(i)] = tmp;
        i = parent(i);
    }
}
//remove and return the task with highest priority(root)
Task* pophigh(PriorityQueue* pq) {
    if (pq->size <= 0) return NULL;

    Task* root = pq->data[0];
    pq->data[0] = pq->data[--pq->size];
    heapiheapi(pq, 0);
    return root;
}

//peek the task with the highest priority
Task* peekhigh(PriorityQueue* pq) {
    return pq->size > 0 ? pq->data[0] : NULL;
}
