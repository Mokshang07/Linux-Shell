#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char history[10][129];
extern int histSeq[10];
extern int histCount;

void add_history(char *cmd, int seq);
void print_history();

int builtin(char **token);

void execute(char **token);

void execute_pipe(char *segment);
