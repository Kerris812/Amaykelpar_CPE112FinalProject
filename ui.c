// ui.c
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ui.h"

#define H   "─"
#define V   "│"
#define TL  "┌"
#define TR  "┐"
#define BL  "└"
#define BR  "┘"
#define TM  "┬"
#define BM  "┴"
#define LM  "├"
#define RM  "┤"
#define CR  "┼"

void headerui(const char* title, int width) {
    printf(TL);

    for (int i = 0; i < width - 2; i++) printf(H);
        printf(TR "\n");

    int pad = (width - 2 - (int)strlen(title)) / 2;
        printf(V);

    for (int i = 0; i < pad; i++) printf(" ");
        printf("%s", title);

    for (int i = pad + (int)strlen(title); i < width - 2; i++) printf(" ");
        printf(V "\n");
        printf(BL);

    for (int i = 0; i < width - 2; i++) printf(H);
        printf(BR "\n\n");
}

void intro(int width) {
    headerui(" Welcome to Amaykelpar To-Do List ", width);
}

void optui(int idx, const char* text, const char* color) {
    printf("%s %2d. %-30s %s\n", color, idx, text, ANSI_RESET);
}

//to print all the output details in table
void tableswel(Task* tasks[], const char* cats[], int count) {
    const int w_cat   = 12;
    const int w_id    =  4;
    const int w_title = 20;
    const int w_pr    =  8;
    const int w_due   = 12;
    const int w_time  =  8;
    const int w_stat  = 12;
    const int w_left  = 18;

    //table top border
    printf(TL);
      for (int i = 0; i < w_cat; i++) printf(H); printf(TM);
      for (int i = 0; i < w_id; i++) printf(H); printf(TM);
      for (int i = 0; i < w_title; i++) printf(H); printf(TM);
      for (int i = 0; i < w_pr; i++) printf(H); printf(TM);
      for (int i = 0; i < w_due; i++) printf(H); printf(TM);
      for (int i = 0; i < w_time; i++) printf(H); printf(TM);
      for (int i = 0; i < w_stat; i++) printf(H); printf(TM);
      for (int i = 0; i < w_left; i++) printf(H); printf(TR "\n");

    //header row
    printf(V "%-*s"  V "%*s"  V "%-*s"  V "%-*s"  V "%-*s"  V "%-*s"  V "%-*s"  V "%-*s"  V "\n",
        w_cat, "Category",
        w_id, "ID",
        w_title, "Title",
        w_pr, "Priority",
        w_due,"DueDate",
        w_time,"DueTime",
        w_stat, "Status",
        w_left, "TimeLeft"
    );

    //table separator
    printf(LM);
    for (int i = 0; i < w_cat; i++) printf(H); printf(CR);
    for (int i = 0; i < w_id; i++) printf(H); printf(CR);
    for (int i = 0; i < w_title; i++) printf(H); printf(CR);
    for (int i = 0; i < w_pr; i++) printf(H); printf(CR);
    for (int i = 0; i < w_due; i++) printf(H); printf(CR);
    for (int i = 0; i < w_time; i++) printf(H); printf(CR);
    for (int i = 0; i < w_stat; i++) printf(H); printf(CR);
    for (int i = 0; i < w_left; i++) printf(H); printf(RM "\n");

    time_t now = time(NULL);

    for (int i = 0; i < count; i++) {
        Task* t = tasks[i];

        //parse due date/time
        struct tm tmD = {0};

        int Y,M,D,h,m;
        sscanf(t->dueDate, "%d-%d-%d", &Y, &M, &D);
        sscanf(t->dueTime, "%d:%d",     &h, &m);
        tmD.tm_year = Y - 1900;
        tmD.tm_mon  = M - 1;
        tmD.tm_mday = D;
        tmD.tm_hour = h;
        tmD.tm_min  = m;
        time_t due  = mktime(&tmD);

        //cal diff and secs
        double diff = difftime(due, now);
        long   sec  = (long)diff;

        //build time-left (blank if complete)
        char buf[32];
        if (t->status == STATUS_COMPLETE) {
            buf[0] = '\0';
        } else if (diff >= 0) {
            int dd = sec/86400; sec %= 86400;
            int hh = sec/3600;  sec %= 3600;
            int mm = sec/60;
            snprintf(buf, sizeof(buf), "%dd %dh %dm", dd, hh, mm);
        } else {
            sec = -sec;
            int dd = sec/86400; sec %= 86400;
            int hh = sec/3600;  sec %= 3600;
            int mm = sec/60;
            snprintf(buf, sizeof(buf), "Over %dd %dh %dm", dd, hh, mm);
        }

        //task status
        const char* st =
            t->status == STATUS_COMPLETE ? "Complete" :
            t->status == STATUS_IN_PROGRESS ? "In-Progress" : "Pending";

        //determine row color that overdue tasks in red, else by status
        const char* col;
        
        if (t->status == STATUS_COMPLETE) {
            col = ANSI_GREEN;
        }
        else if (diff < 0) {
            col = ANSI_RED;
        }
        else if (t->status == STATUS_IN_PROGRESS) {
            col = ANSI_CYAN;
        }
        else {
            col = ANSI_YELLOW;
        }

        //task priority
        const char* prStr =
            t->priority == PRI_HIGH ? "High":
            t->priority == PRI_MEDIUM ? "Medium":
            t->priority == PRI_LOW ? "Low": "Unknown";

        //print the row
        printf("%s" V "%-*s"  V "%*d"  V "%-*s"  V "%-*s"  V "%-*s"  V "%-*s"  V "%-*s"  V "%-*s"  V ANSI_RESET "\n",
            col,
            w_cat,   cats[i],
            w_id,    t->id,
            w_title, t->title,
            w_pr,    prStr,
            w_due,   t->dueDate,
            w_time,  t->dueTime,
            w_stat,  st,
            w_left,  buf
        );
    }

    //table bottom border
    printf(BL);
      for (int i = 0; i < w_cat;   i++) printf(H); printf(BM);
      for (int i = 0; i < w_id;    i++) printf(H); printf(BM);
      for (int i = 0; i < w_title; i++) printf(H); printf(BM);
      for (int i = 0; i < w_pr;    i++) printf(H); printf(BM);
      for (int i = 0; i < w_due;   i++) printf(H); printf(BM);
      for (int i = 0; i < w_time;  i++) printf(H); printf(BM);
      for (int i = 0; i < w_stat;  i++) printf(H); printf(BM);
      for (int i = 0; i < w_left;  i++) printf(H); printf(BR "\n");
}
