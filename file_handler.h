//file_handler.h
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "task_manager.h"

void saveToCSV(Category* cats, const char* filename);
void loadFromCSV(Category** cats, const char* filename);

#endif
