//task_manager.h
#include "priority_queue.h"

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#define MAX_TITLE 40
#define MAX_DESC 999
#define MAX_DATE 11
#define MAX_TIME 6
#define MAX_CATEGORY 50

#define STATUS_PENDING 0
#define STATUS_COMPLETE 1
#define STATUS_IN_PROGRESS 2

#define PRI_HIGH     1
#define PRI_MEDIUM   2
#define PRI_LOW      3

//task structure
typedef struct Task {
    int    id;
    char   title[MAX_TITLE];
    char   description[MAX_DESC];
    char   dueDate[MAX_DATE];
    char   dueTime[MAX_TIME];
    int    priority;   
    int    status;      
    struct Task* next;
} Task;

//category structure
typedef struct Category {
    char name[MAX_CATEGORY];
    PriorityQueue pq;
    struct Category* next;
} Category;


Category* catcreateml(Category** cats, const char* name);
void taskcreate(Category** cats);
void viewTasks(Category* cats);
void viewTodayTasks(Category* cats);
void updateTask(Category* cats);
void deleteTask(Category** cats);
void restoreTask(Category** cats);
void markStatus(Category* cats);
void sortTasksByDate(Category* cats);
void sortTasksByPriority(Category* cats);
void viewCompletedTasks(Category* cats);
void showStatistics(Category* cats);
void viewTaskDescription(Category* categories);
void viewTasksInCategory(Category* categories);
void renameCategory(Category* categories);
void mergeCategories(Category** categories);
void saveToCSV(Category* cats, const char* filename);
void loadFromCSV(Category** cats, const char* filename);
void duplicateTask(Category** categories);
void sortTasksByName(Category* categories);

//to compare two date and time
int sooner(const char* d1, const char* t1,
                 const char* d2, const char* t2);


                 
#endif
