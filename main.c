//main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//libraries we developed by using header
#include "task_manager.h"
#include "utils.h"
#include "ui.h"
#include "file_handler.h"

int main(void) {
    
    system("chcp 65001"); //set to UTF-8 to displaay box drawing charactures correctly
    
    Category* categories = NULL;
    const char* filename = "tasks.csv"; //cvs file must be in the name of "tasks.csv"

    srand((unsigned)time(NULL));
    loadFromCSV(&categories, filename);

    int choice;
    do {
        cleanterminal(); //clear terminal
        intro(60); //intro with group title
        toEnter();

        //main menu that colored by using ANSI colors
        optui(1, "Create Task", ANSI_YELLOW);
        optui(2, "Update Task",ANSI_YELLOW);
        optui(3, "Duplicate Tasks",ANSI_YELLOW);
        optui(4, "Delete Task",ANSI_YELLOW);
        optui(5, "Restore Task", ANSI_YELLOW);
        optui(6, "Rename Category", ANSI_YELLOW);
        optui(7, "Merge Categories", ANSI_YELLOW);
        optui(8, "Mark Task Status",ANSI_YELLOW);
        optui(9, "View All Tasks",ANSI_YELLOW);
        optui(10,"View Completed Tasks", ANSI_YELLOW);
        optui(11,"View Tasks Due 24hrs", ANSI_YELLOW);
        optui(12,"View Tasks in Same Category", ANSI_YELLOW);
        optui(13,"View Tasks Description", ANSI_YELLOW);
        optui(14,"View Statistics",ANSI_YELLOW);
        optui(15,"Sort by Date", ANSI_YELLOW);
        optui(16,"Sort by Priority", ANSI_YELLOW);
        optui(17,"Sort by Name", ANSI_YELLOW);

        optui(0, "Exit", ANSI_BG_RED);

        //Option must be in the range
        choice = inRange(0, 17, "Select Option: ");

        //All the menu options are mentions here
        switch(choice) {
            case 1: taskcreate(&categories);          
                break;
            case 2: viewTasks(categories); 
                    updateTask(categories);          
                break;
            case 3: viewTasks(categories); 
                    duplicateTask(&categories);    
                break;
            case 4: viewTasks(categories); 
                    deleteTask(&categories);          
                break;
            case 5: viewTasks(categories); 
                    restoreTask(&categories);         
                break;
            case 6: viewTasks(categories); 
                    renameCategory(categories);       
                break;
            case 7: viewTasks(categories); 
                    mergeCategories(&categories);     
                break;
            case 8: viewTasks(categories); 
                    markStatus(categories);           
                break;
            case 9: viewTasks(categories);            
                break;
            case 10: viewCompletedTasks(categories);         
                break;
            case 11: viewTodayTasks(categories); 
                break;
            case 12: viewTasks(categories);
                     viewTasksInCategory(categories);       
                break;
            case 13: viewTasks(categories);
                     viewTaskDescription(categories);       
                break;
            case 14: viewTasks(categories);
                     showStatistics(categories);      
                break;
            case 15: sortTasksByDate(categories);  
                break;
            case 16: sortTasksByPriority(categories);   
                break;
            case 17: sortTasksByName(categories);
        }

        //update every action to csv
        saveToCSV(categories, filename);

        if (choice != 0) toEnter();

    } 

    while (choice != 0);
    return 0;
}
