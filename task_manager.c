//task_manager.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "task_manager.h"
#include "utils.h"
#include "ui.h"
#include "file_handler.h"
#include "priority_queue.h"

//generate 4-digit random ID for tasks
static int genRandomID(void) {
    return 1000 + rand() % 9000;
}

static Category* findCategory(Category* cats, const char* name) {
    for (Category* c = cats; c; c = c->next) {
        if (strcmp(c->name, name) == 0) {
            return c;
        }
    }
    return NULL;
}

extern int genRandomID(void);
static Task* findById(Category* cats, int id, Category** outCat, int* outIdx);

typedef struct NameNode {
    Task*            task;
    const char*      catName;
    struct NameNode* next;
} NameNode;

//compare two date+time pairs
int sooner(const char* d1, const char* t1,
                 const char* d2, const char* t2) {
    struct tm m1 = {0}, m2 = {0};

    sscanf(d1, "%d-%d-%d", &m1.tm_year, &m1.tm_mon, &m1.tm_mday);
    sscanf(t1, "%d:%d",     &m1.tm_hour, &m1.tm_min);
    sscanf(d2, "%d-%d-%d", &m2.tm_year, &m2.tm_mon, &m2.tm_mday);
    sscanf(t2, "%d:%d",     &m2.tm_hour, &m2.tm_min);

    m1.tm_year -= 1900; m1.tm_mon -= 1;
    m2.tm_year -= 1900; m2.tm_mon -= 1;

    time_t time1 = mktime(&m1);
    time_t time2 = mktime(&m2);

    if (time1 < time2) return -1;
    if (time1 > time2) return  1;

    return 0;
}

//create category
Category* catcreateml(Category** cats, const char* name) {
    for (Category* c = *cats; c; c = c->next)
        if (strcmp(c->name, name) == 0)
            return c;

    Category* nc = malloc(sizeof(Category));
    strcpy(nc->name, name);
    initQueue(&nc->pq);
    nc->next = *cats;
    *cats = nc;
    return nc;
}

//create task
void taskcreate(Category** cats) {
    char title[MAX_TITLE], desc[MAX_DESC];
    char dueDate[MAX_DATE], dueTime[MAX_TIME], catName[MAX_CATEGORY];
    int  choice;

    inputu(catName, MAX_CATEGORY, "Category: ");
    Category* cat = catcreateml(cats, catName);

    inputu(title, MAX_TITLE, "Title: ");
    inputu(desc,  MAX_DESC,  "Description: ");

    //due date
    while (1) {
        inputu(dueDate, MAX_DATE, "Due date in format (YYYY-MM-DD): ");
        if (!dateValidCase(dueDate)) {
            printf("Invalid date.\n"); 
            continue;
        }
        if (!dateTimeOffsetValid(dueDate, "23:59", 0)) {
            printf("Date entered is in the past.\n"); 
            continue;
        }
        break;
    }

    //due time
    while (1) {
        inputu(dueTime, MAX_TIME, "Due time in format (HH:MM): ");
        if (!timeValidCase(dueTime)) {
            printf("Invalid time.\n");
            continue;
        }
        if (!dateTimeOffsetValid(dueDate, dueTime, 30)) {
            printf("Set a time at least 30 minutes later.\n");
            continue;
        }
        break;
    }

    //priority
    printf("1) High\n2) Medium\n3) Low\n");
    choice = inRange(1, 3, "Priority: ");

    int pr = (choice == 1 ? PRI_HIGH : choice == 2 ? PRI_MEDIUM : PRI_LOW);

    Task* t = malloc(sizeof(Task));
    t->id          = genRandomID();

    strcpy(t->title,       title);
    strcpy(t->description, desc);
    strcpy(t->dueDate,     dueDate);
    strcpy(t->dueTime,     dueTime);

    t->priority    = pr;
    t->status      = STATUS_PENDING;
    t->next        = NULL;

    bubblein(&cat->pq, t);
    printf("Task added \"%s\" with ID %d.\n", title, t->id);
}

//view all tasks in table
void viewTasks(Category* cats) {
    Task* arr[MAX_TASKS * 10];
    const char* catArr[MAX_TASKS * 10];
    int c = 0;

    for (Category* cat = cats; cat; cat = cat->next) {
        for (int i = 0; i < cat->pq.size; i++) {
            arr[c]    = cat->pq.data[i];
            catArr[c] = cat->name;
            c++;
        }
    }

    //sort by priority & date
    for (int i = 0; i < c - 1; i++) {
        for (int j = 0; j < c - 1 - i; j++) {
            if (compareTask(arr[j], arr[j + 1]) > 0) {
                Task* tmp    = arr[j];
                arr[j]       = arr[j + 1];
                arr[j + 1]   = tmp;
                const char* cn = catArr[j];
                catArr[j]      = catArr[j + 1];
                catArr[j + 1]  = cn;
            }
        }
    }

    if (c) {
        tableswel(arr, catArr, c);
    } else {
        printf("No tasks is in to-do list yet.\n");
    }
}

//view today’s tasks
void viewTodayTasks(Category* cats) {
    Task* arr[MAX_TASKS * 10];
    const char* catArr[MAX_TASKS * 10];
    int c = 0;
    time_t now = time(NULL);

    for (Category* cat = cats; cat; cat = cat->next) {
        for (int i = 0; i < cat->pq.size; i++) {
            Task* t = cat->pq.data[i];
            if (t->status == STATUS_COMPLETE)
            continue;

            struct tm tmD = {0};
            int Y,M,D,h,m;

            sscanf(t->dueDate, "%d-%d-%d", &Y,&M,&D);
            sscanf(t->dueTime, "%d:%d",     &h,&m);

            tmD.tm_year = Y - 1900;
            tmD.tm_mon  = M - 1;
            tmD.tm_mday = D;
            tmD.tm_hour = h;
            tmD.tm_min  = m;

            time_t due = mktime(&tmD);
            double diff = difftime(due, now);

            if (diff >= 0 && diff <= 86400) {
                arr[c]    = t;
                catArr[c] = cat->name;
                c++;
            }
        }
    }

    if (c) {
        tableswel(arr, catArr, c);
    } else {
        printf("No tasks will be dued in the next 24 hours, go chill bro.\n");
    }
}

//helpers funciton for delete/restore
static Task* findById(Category* cats, int id, Category** outCat, int* outIdx) {
    for (Category* cat = cats; cat; cat = cat->next) {
        for (int i = 0; i < cat->pq.size; i++) {
            if (cat->pq.data[i]->id == id) {
                if (outCat) *outCat = cat;
                if (outIdx) *outIdx = i;

                return cat->pq.data[i];
            }
        }
    }
    return NULL;
}

static void removeAt(PriorityQueue* pq, int idx) {
    Task* tmpArr[MAX_TASKS];
    int c = 0;
    for (int i = 0; i < pq->size; i++) {
        if (i != idx) tmpArr[c++] = pq->data[i];
    }
    initQueue(pq);
    for (int i = 0; i < c; i++) {
        bubblein(pq, tmpArr[i]);
    }
}

//update task
void updateTask(Category* cats) {
    int id = inRange(1000, 9999, "Task ID: ");
    Category* cat = NULL;
    int idx;
    Task* t = findById(cats, id, &cat, &idx);
    if (!t) { printf("Not found the task.\n"); return; }

    inputu(t->title,       MAX_TITLE, "New title: ");
    inputu(t->description, MAX_DESC,  "New description: ");
    
    //due date
    while (1) {
        inputu(t->dueDate, MAX_DATE, "Due date is format (YYYY-MM-DD): ");
        if (!dateValidCase(t->dueDate)) {
            printf("Invalid date.\n"); continue;
        }
        if (!dateTimeOffsetValid(t->dueDate, "23:59", 0)) {
            printf("Date is in the  past.\n"); continue;
        }
        break;
    }
    //due time
    while (1) {
        inputu(t->dueTime, MAX_TIME, "Due time in format (HH:MM): ");
        if (!timeValidCase(t->dueTime)) {
            printf("Invalid time.\n"); continue;
        }
        if (!dateTimeOffsetValid(t->dueDate, t->dueTime, 30)) {
            printf("Time must be less than 30min ahead.\n"); continue;
        }
        break;
    }
    printf("Task is updated.\n");
}

//deleted-stack
static Task* deletedStack = NULL;

//delete task
void deleteTask(Category** cats) {
    int id = inRange(1000, 9999, "ID: ");
    Category* cat = NULL;
    int idx;

    Task* t = findById(*cats, id, &cat, &idx);
    if (!t) {
        printf("No task is find with ID %d.\n", t->id);
        return;
    }

    printf("Delete \"%s\"? [%sYeah%s or %sNah%s]: ",
           t->title,
           ANSI_RED, 
           ANSI_RESET,
           ANSI_GREEN,
           ANSI_RESET);

    char ans[4];
    inputu(ans, sizeof(ans), "");

    if (ans[0] != 'y' && ans[0] != 'Y') {
        printf("Cancelled task deletion.\n");
        return;
    }

    removeAt(&cat->pq, idx);
    t->next = deletedStack;
    deletedStack = t;
    printf("Task is Deleted.\n");
}


//restore task
void restoreTask(Category** cats) {
    if (!deletedStack) {
        printf("No deleted tasks.\n");
        return;
    }

    Task* arr[100];
    const char* catArr[100];
    int c = 0;

    for (Task* p = deletedStack; p; p = p->next) {
        arr[c]    = p;
        catArr[c]= "<deleted>";
        c++;
    }

    printf("\nDeleted Tasks: \n");
    for (int i = 0; i < c; i++) {
        printf("%2d) ID %d  %s\n", i+1, arr[i]->id, arr[i]->title);
    }

    int ch = inRange(1, c, "Pick: ");
    Task* t = arr[ch-1];

    //pop from stack
    if (deletedStack == t){
        deletedStack = t->next;
    }
    else {
        Task* p = deletedStack;
        while (p->next != t) p = p->next;
        p->next = t->next;
    }

    char name[MAX_CATEGORY];
    inputu(name, MAX_CATEGORY, "Category: ");
    Category* cat = catcreateml(cats, name);
    bubblein(&cat->pq, t);

    printf("Restored Task.\n");
}

//mark status if the task is done or in progress
void markStatus(Category* cats) {
    int id = inRange(1000, 9999, "Enter Task ID: ");

    //look it up
    Category* cat = NULL;
    Task*     f   = findById(cats, id, &cat, NULL);
    if (!f) {
        printf("Task with ID %d not found.\n", id);
        return;
    }
    printf(ANSI_GREEN  "1) Mark Task as Complete\n" ANSI_RESET);
    printf(ANSI_BLUE "2) Mark Task as In-Progress\n" ANSI_RESET);

    int choice = inRange(1, 2, "Choice: ");

    f->status = (choice == 1 ? STATUS_COMPLETE : STATUS_IN_PROGRESS);

    printf("Task #%d \"%s\" status set to %s.\n",
           f->id,
           f->title,
           (f->status == STATUS_COMPLETE ? "Complete" : "In-Progress"));
}

//sort by date
void sortTasksByDate(Category* cats) {
    Task* arr[MAX_TASKS * 10];
    const char* catArr[MAX_TASKS * 10];
    int c = 0;

    for (Category* cat = cats; cat; cat = cat->next) {
        for (int i = 0; i < cat->pq.size; i++) {
            arr[c]    = cat->pq.data[i];
            catArr[c] = cat->name;
            c++;
        }
    }
    for (int i = 0; i < c - 1; i++) {
        for (int j = 0; j < c - 1 - i; j++) {
            if (sooner(arr[j]->dueDate, arr[j]->dueTime,
                             arr[j+1]->dueDate, arr[j+1]->dueTime) > 0) {
                Task* tmp     = arr[j];
                arr[j]        = arr[j+1];
                arr[j+1]      = tmp;
                const char* cn = catArr[j];
                catArr[j]      = catArr[j+1];
                catArr[j+1]    = cn;
            }
        }
    }

    if (c) {
        tableswel(arr, catArr, c);
    } else {
        printf("There is no tasks to display, yay.\n");
    }
}

//sort by priority
void sortTasksByPriority(Category* cats) {
    Task* arr[MAX_TASKS * 10];
    const char* catArr[MAX_TASKS * 10];
    int c = 0;

    for (Category* cat = cats; cat; cat = cat->next) { 
        for (int i = 0; i < cat->pq.size; i++) {
            arr[c]    = cat->pq.data[i];
            catArr[c] = cat->name;
            c++;
        }
    }
    for (int i = 0; i < c - 1; i++) {
        for (int j = 0; j < c - 1 - i; j++) {
            if (compareTask(arr[j], arr[j+1]) > 0) {
                Task* tmp     = arr[j];
                arr[j]        = arr[j+1];
                arr[j+1]      = tmp;
                const char* cn = catArr[j];
                catArr[j]      = catArr[j+1];
                catArr[j+1]    = cn;
            }
        }
    }

    if (c) {
        tableswel(arr, catArr, c);
    } else {
        printf("There is no tasks to display.\n");
    }
}

//view completed tasks only
void viewCompletedTasks(Category* cats) {
    Task* arr[MAX_TASKS * 10];
    const char* catArr[MAX_TASKS * 10];
    int c = 0;

    for (Category* cat = cats; cat; cat = cat->next) {
        for (int i = 0; i < cat->pq.size; i++) {
            if (cat->pq.data[i]->status == STATUS_COMPLETE) {
                arr[c]    = cat->pq.data[i];
                catArr[c] = cat->name;
                c++;
            }
        }
    }

    if (c) {
        tableswel(arr, catArr, c);
    } else {
        printf("There is no completed tasks. Come on man, do something productive.\n");
    }
}

//statistics of all tasks staus
void showStatistics(Category* cats) {
    int done = 0, inP = 0, pend = 0, ov = 0;
    time_t now = time(NULL);

    for (Category* cat = cats; cat; cat = cat->next) {
        for (int i = 0; i < cat->pq.size; i++) {
            Task* t = cat->pq.data[i];

            if (t->status == STATUS_COMPLETE) {
                done++;
            } else if (t->status == STATUS_IN_PROGRESS) {
                inP++;
            } else {
                struct tm tmD = {0};
                int Y,M,D,h,m;

                sscanf(t->dueDate, "%d-%d-%d",&Y,&M,&D);
                sscanf(t->dueTime, "%d:%d",&h,&m);
                tmD.tm_year=Y-1900; tmD.tm_mon=M-1;
                tmD.tm_mday=D; tmD.tm_hour=h; tmD.tm_min=m;
                time_t due = mktime(&tmD);

                if (difftime(due, now) < 0) ov++;
                else pend++;
            }
        }
    }

    printf("\nTask Summary\n");
    printf(ANSI_GREEN  "Complete:     %d\n" ANSI_RESET, done);
    printf(ANSI_BLUE   "In-Progress:  %d\n" ANSI_RESET, inP);
    printf(ANSI_YELLOW "Pending:      %d\n" ANSI_RESET, pend);
    printf(ANSI_RED    "Overdue:      %d\n" ANSI_RESET, ov);
}

void viewTasksInCategory(Category* cats) {
    char catName[MAX_CATEGORY];
    inputu(catName, MAX_CATEGORY, "Enter category name: ");

    Category* cat = findCategory(cats, catName);

    if (!cat || cat->pq.size == 0) {
        printf("No tasks found in category '%s'.\n", catName);
        return;
    }

    //gather ptr plus parallel category names
    Task*       arr[ MAX_TASKS ];
    const char* catArr[ MAX_TASKS ];
    int         count = 0;

    for (int i = 0; i < cat->pq.size; i++) {
        arr[count]    = cat->pq.data[i];
        catArr[count] = cat->name;
        count++;
    }

    printf("\nTasks in '%s': \n", catName);
    tableswel(arr, catArr, count);
}

void renameCategory(Category* categories){
    char oldName[MAX_CATEGORY];
    char newName[MAX_CATEGORY];

    inputu(oldName, MAX_CATEGORY, "Enter existing category name: ");
    Category* cat = findCategory(categories, oldName);

    if (!cat) {
        printf("Category '%s' is not found.\n", oldName);
        return;
    }

    inputu(newName, MAX_CATEGORY, "Enter new category name: ");

    if (findCategory(categories, newName)) {
        printf("Category '%s' is already existed.\nChoose a different name or merge it.\n", newName);
        return;
    }

    strcpy(cat->name, newName);
    printf("Renamed category '%s' → '%s'.\n", oldName, newName);
}

//merge one category into another, then delete the old one
void mergeCategories(Category** categories) {
    char srcName[MAX_CATEGORY];
    char dstName[MAX_CATEGORY];

    inputu(srcName, MAX_CATEGORY, "Enter source category to merge from: ");
    inputu(dstName, MAX_CATEGORY, "Enter destination category to merge into: ");

    if (strcmp(srcName, dstName) == 0) {
        printf("Cannot merge a category into itself.\n");
        return;
    }

    Category* src = findCategory(*categories, srcName);
    if (!src) {
        printf("Source category '%s' is not found.\n", srcName);
        return;
    }

    Category* dst = findCategory(*categories, dstName);
    if (!dst) {
        printf("Destination category '%s' is not found.\n", dstName);
        return;
    }

    //move every task ptr from src.pq into dst.pq
    for (int i = 0; i < src->pq.size; i++) {
        bubblein(&dst->pq, src->pq.data[i]);
    }

    //clear the source queue
    src->pq.size = 0;

    //remove node from the categories linked list
    Category* prev = NULL;
    Category* cur  = *categories;
    while (cur && cur != src) {
        prev = cur;
        cur  = cur->next;
    }

    if (!cur) {
        //shouldn't happen
    } else if (!prev) {
        //head of list
        *categories = src->next;
    } else {
        prev->next = src->next;
    }

    free(src);
    printf("Merged '%s' into '%s' and deleted '%s'.\n", srcName, dstName, srcName);
}

void duplicateTask(Category** categories) {
    int id = inRange(1000, 9999, "Enter ID of task to duplicate: ");
    Category* cat = NULL;
    int idx;

    Task* orig = findById(*categories, id, &cat, &idx);
    if (!orig) {
        printf("Task with ID %d is not found.\n", id);
        return;
    }

    Task* dup = malloc(sizeof(Task));
    if (!dup) {
        printf("Failed.\n");
        return;
    }

    //copy all details and give new id
    dup->id          = genRandomID();
    strcpy(dup->title,       orig->title);
    strcpy(dup->description, orig->description);
    strcpy(dup->dueDate,     orig->dueDate);
    strcpy(dup->dueTime,     orig->dueTime);
    dup->priority    = orig->priority;
    dup->status      = orig->status;
    dup->next        = NULL;

    //insert into the same category’s priority queue
    bubblein(&cat->pq, dup);

    printf("Duplicated \"%s\" as new task ID %d in category \"%s\".\n",
           orig->title, dup->id, cat->name);
}

void viewTaskDescription(Category* categories) {
    int id = inRange(1000, 9999, "Enter Task ID: ");

    Category* cat = NULL;
    Task* t = findById(categories, id, &cat, NULL);
    if (!t) {
        printf("Task with ID %d not found.\n", id);
        return;
    }

    printf("Title      : %s\n", t->title);
    printf("Description: %s\n", t->description);
}

void sortTasksByName(Category* cats) {
    NameNode* head = NULL;
    int total = 0;

    //build sorted circular list by name
    for (Category* c = cats; c; c = c->next) {
        for (int i = 0; i < c->pq.size; i++) {
            Task* t = c->pq.data[i];
            NameNode* nn = malloc(sizeof(NameNode));
            nn->task    = t;
            nn->catName = c->name;
            nn->next    = NULL;
            total++;

            if (!head) {
                //first node points to itself
                head = nn;
                nn->next = head;
            } else {
                //if it is belongs before head
                if (strcasecmp(t->title, head->task->title) < 0) {
                    //find tail
                    NameNode* tail = head;
                    while (tail->next != head) tail = tail->next;
                    tail->next = nn;
                    nn->next   = head;
                    head = nn;
                } else {
                    //find
                    NameNode* cur = head;
                    while (cur->next != head &&
                           strcasecmp(cur->next->task->title, t->title) < 0) {
                        cur = cur->next;
                    }
                    nn->next    = cur->next;
                    cur->next   = nn;
                }
            }
        }
    }

    if (total == 0) {
        printf("No tasks to sort.\n");
        return;
    }

    //go through circular list to fill arrays
    Task*       arr[total];
    const char* catArr[total];
    int         idx = 0;
    NameNode*   cur = head;
    do {
        arr[idx]    = cur->task;
        catArr[idx] = cur->catName;
        idx++;
        cur = cur->next;
    } while (cur != head);

    printf("\nTasks Sorted by Name\n");
    tableswel(arr, catArr, total);

    //free nodes
    cur = head->next;
    while (cur != head) {
        NameNode* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    
    free(head);
}
