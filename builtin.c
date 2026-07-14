#include "header.h"

int builtin(char **token)
{
        if(strcmp(token[0], "quit") == 0)
                exit(0);

        else if(strcmp(token[0], "curpid") == 0)
        {
                printf("curpid=%d\n", getpid());
                return 1;
        }

        else if(strcmp(token[0], "ppid") == 0)
        {
                printf("ppid=%d\n", getppid());
                return 1;
        }

        else if(strcmp(token[0], "cd") == 0)
        {
                if(token[1] == NULL)
                        printf("cd: missing argument\n");

                else if(chdir(token[1]) != 0)
                        perror("cd failed");

                else
                {
                        char buf[256];

                        getcwd(buf, sizeof(buf));
                        printf("Current directory: %s\n", buf);
                }

                return 1;
        }

        else if(strcmp(token[0], "hist") == 0)
        {
                print_history();
                return 1;
        }

        return 0;
}
