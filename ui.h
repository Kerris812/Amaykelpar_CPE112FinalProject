//ui.h
#ifndef UI_H
#define UI_H

#include <time.h>
#include "task_manager.h"

//color defination
#define ANSI_RESET    "\x1b[0m"
#define ANSI_RED      "\x1b[31m"
#define ANSI_GREEN    "\x1b[32m"
#define ANSI_YELLOW   "\x1b[33m"
#define ANSI_BLUE     "\x1b[34m"
#define ANSI_CYAN     "\x1b[36m"
#define ANSI_BYELLOW "\x1b[43m"
#define ANSI_BCYAN   "\x1b[46m"
#define ANSI_BRED "\x1b[91m"
#define ANSI_BG_RED   "\x1b[41m"
#define ANSI_BG_BRED  "\x1b[101m"

void headerui(const char* title, int width);
void intro(int width);
void optui(int idx, const char* text, const char* color);
void tableswel(Task* tasks[], const char* cats[], int count);

#endif
