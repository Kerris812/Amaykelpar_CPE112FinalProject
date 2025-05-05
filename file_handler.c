//file_handler.c
#include "file_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveToCSV(Category* cats, const char* filename) {
    FILE* fp = fopen(filename, "w");

    if (!fp) { 
        perror("fopen"); 
        return; 
    }
    //loop all cats
    for (Category* cat=cats; cat; cat=cat->next)
    //loop all tasks
        for (int i=0; i<cat->pq.size; i++) {
            Task* t = cat->pq.data[i];
            fprintf(fp, "%s,%d,%s,%s,%d,%s,%s,%d\n",
                cat->name, 
                t->id, 
                t->title, 
                t->description,
                t->priority, 
                t->dueDate, 
                t->dueTime, 
                t->status);
        }
    fclose(fp);
}

void loadFromCSV(Category** cats, const char* filename) {
    FILE* fp = fopen(filename, "r");

    if (!fp) return;

    char line[1024];
    //read csv all
    while (fgets(line, sizeof(line), fp)) {
        if (line[0]=='\n') continue;

        //temp var
        char catName[MAX_CATEGORY], title[MAX_TITLE], desc[MAX_DESC], dueDate[MAX_DATE], dueTime[MAX_TIME];

        int id, pr, st;
        if (sscanf(line,"%[^,],%d,%[^,],%[^,],%d,%[^,],%[^,],%d", catName,&id,title,desc,&pr,dueDate,dueTime,&st) != 8) continue;

        //allocate and fill new task
        Task* t = malloc(sizeof(Task));
        t->id = id;
        strcpy(t->title, title);
        strcpy(t->description, desc);
        t->priority = pr;
        strcpy(t->dueDate, dueDate);
        strcpy(t->dueTime, dueTime);
        t->status = st;
        t->next = NULL;

        //create category and insert task into its priority queue
        Category* cat = catcreateml(cats, catName);
        bubblein(&cat->pq, t);
    }
    fclose(fp);
}
