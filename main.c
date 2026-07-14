#include "header.h"

int main(int argc, char **argv)
{
        char cmd[129], host[20], c1[128];
        int c = 1, i, j = 0;
        char *token[20];

        if(argc > 1 && strcmp(argv[1], "-c") == 0)
        {
                for(i = 2; i < argc; i++)
                {
                        strcat(c1, argv[i]);

                        if(i != argc - 1)
                                strcat(c1, " ");
                }

                printf("Executing command: %s\n", c1);
                system(c1);
                return 0;
        }

        gethostname(host, sizeof(host));

        while(1)
        {
                printf("<%d %s>", c, host);
                scanf(" %128[^\n]", cmd);

                add_history(cmd, c);

                char *save1;
                char *segment = strtok_r(cmd, ";", &save1);

                while(segment != NULL)
                {
                        while(*segment == ' ' || *segment == '\t')
                                segment++;

                        if(*segment == '\0')
                        {
                                segment = strtok_r(NULL, ";", &save1);
                                continue;
                        }

                        if(strchr(segment, '|'))
                        {
                                execute_pipe(segment);
                        }
                        else
                        {
                                j = 0;
                                token[j] = strtok(segment, " \t");

                                while(token[j] != NULL)
                                {
                                        j++;
                                        token[j] = strtok(NULL, " \t");
                                }

                                if(token[0] == NULL)
                                {
                                        segment = strtok_r(NULL, ";", &save1);
                                        continue;
                                }

                                if(!builtin(token))
                                        execute(token);
                        }

                        segment = strtok_r(NULL, ";", &save1);
                }

                c++;
        }

        return 0;
}
