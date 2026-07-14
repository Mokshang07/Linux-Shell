#include "header.h"

void execute_pipe(char *segment)
{
        char *pipeCmd[10];
        char *token[20];
        int ncmd = 0;
        int fd[2];
        int in = 0;
        int i, j;

        pipeCmd[ncmd] = strtok(segment, "|");

        while(pipeCmd[ncmd] != NULL)
        {
                while(*pipeCmd[ncmd] == ' ')
                        pipeCmd[ncmd]++;

                ncmd++;
                pipeCmd[ncmd] = strtok(NULL, "|");
        }

        for(i = 0; i < ncmd; i++)
        {
                pipe(fd);

                if(fork() == 0)
                {
                        dup2(in, 0);

                        if(i != ncmd - 1)
                                dup2(fd[1], 1);

                        close(fd[0]);
                        close(fd[1]);

                        j = 0;
                        token[j] = strtok(pipeCmd[i], " \t");

                        while(token[j] != NULL)
                        {
                                j++;
                                token[j] = strtok(NULL, " \t");
                        }

                        execvp(token[0], token);
                        perror("execvp");
                        exit(1);
                }

                close(fd[1]);

                if(in != 0)
                        close(in);

                in = fd[0];
        }

        for(i = 0; i < ncmd; i++)
                wait(NULL);

        if(in != 0)
                close(in);
}
