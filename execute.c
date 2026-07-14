#include "header.h"

void execute(char **token)
{
        if(fork() == 0)
        {
                int i;
                int in = -1, out = -1;

                for(i = 0; token[i] != NULL; i++)
                {
                        if(strcmp(token[i], "<") == 0)
                        {
                                in = open(token[i + 1], O_RDONLY);

                                if(in < 0)
                                {
                                        perror("open");
                                        exit(1);
                                }

                                dup2(in, 0);
                                close(in);
                                token[i] = NULL;
                        }

                        else if(strcmp(token[i], ">") == 0)
                        {
                                out = open(token[i + 1],
                                           O_WRONLY | O_CREAT | O_TRUNC,
                                           0644);

                                if(out < 0)
                                {
                                        perror("open");
                                        exit(1);
                                }

                                dup2(out, 1);
                                close(out);
                                token[i] = NULL;
                        }
                }

                execvp(token[0], token);
                perror("exec");
                exit(1);
        }
        else
        {
                wait(NULL);
        }
}
