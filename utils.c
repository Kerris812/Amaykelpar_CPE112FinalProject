//utils.c
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//clear terminal
void cleanterminal(void) {
    printf("\x1b[2J\x1b[H");
}

//wait until user hits Enter button
void toEnter(void) {
    printf("Press Enter to continue!");
    while (getchar() != '\n');
}

//valid if an only if the option is in the range
int inRange(int min, int max, const char* prompt) {
    char buf[64];
    int  val;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max) return val;
        printf("Invalid. Enter a number between %d and %d.\n", min, max);
    }
}

void inputu(char* buf, int size, const char* prompt) {
    printf("%s", prompt);
    if (!fgets(buf, size, stdin)) {
        buf[0] = '\0';
        return;
    }
    
    size_t ln = strlen(buf);
    if (ln > 0 && buf[ln - 1] == '\n') {
        buf[ln - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF){ 

        }
    }
}

int dateValidCase(const char* date) {
    int y, m, d;
    if (sscanf(date, "%d-%d-%d", &y, &m, &d) != 3) return 0;
    struct tm tm = {0};
    tm.tm_year = y - 1900;
    tm.tm_mon  = m - 1;
    tm.tm_mday = d;
    tm.tm_isdst = -1;
    time_t t = mktime(&tm);
    return t != (time_t)-1
        && tm.tm_mday == d
        && tm.tm_mon  == m - 1
        && tm.tm_year== y - 1900;
}

int timeValidCase(const char* timeStr) {
    int h, m;
    if (sscanf(timeStr, "%d:%d", &h, &m) != 2) return 0;
    return h >= 0 && h < 24 && m >= 0 && m < 60;
}

int dateTimeOffsetValid(const char* date, const char* timeStr, int offsetMinutes) {
    if (!dateValidCase(date) || !timeValidCase(timeStr)) return 0;
    int y, mo, d, h, mi;
    sscanf(date,    "%d-%d-%d", &y, &mo, &d);
    sscanf(timeStr, "%d:%d",    &h, &mi);
    struct tm tm = {0};
    tm.tm_year = y - 1900;
    tm.tm_mon  = mo - 1;
    tm.tm_mday = d;
    tm.tm_hour = h;
    tm.tm_min  = mi;
    tm.tm_sec  = 0;
    tm.tm_isdst= -1;
    time_t due = mktime(&tm), now = time(NULL);
    return difftime(due, now) >= offsetMinutes * 60;
}
