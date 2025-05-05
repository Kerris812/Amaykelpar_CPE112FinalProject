//utils.h
#ifndef UTILS_H
#define UTILS_H

#include <time.h>

int   inRange(int min, int max, const char* prompt);
void  inputu(char* buf, int size, const char* prompt);
int   dateValidCase(const char* date);
int   timeValidCase(const char* timeStr);
int   dateTimeOffsetValid(const char* date, const char* timeStr, int offsetMinutes);
void  cleanterminal(void);
void  toEnter(void);

#endif
