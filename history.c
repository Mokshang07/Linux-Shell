#include "header.h"

char history[10][129];
int histSeq[10];
int histCount = 0;

void add_history(char *cmd, int seq)
{
        strcpy(history[histCount % 10], cmd);
        histSeq[histCount % 10] = seq;
        histCount++;
}

void print_history()
{
        int i;
        int start = histCount > 10 ? histCount - 10 : 0;

        for(i = start; i < histCount; i++)
                printf("%d %s\n", histSeq[i % 10], history[i % 10]);
}
