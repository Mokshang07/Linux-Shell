#include "header.h"

int main(int argc,char **argv)
{
	char cmd[129],host[20],c1[128]="";
	int c=1,i,j=0;
	char *token[20];
	char history[10][129];
	int histSeq[10];
	int histCount=0;

	if(argc>1 && strcmp(argv[1],"-c")==0)
	{
		char c1[128]="";

		for(i=2;i<argc;i++)
		{
			strcat(c1,argv[i]);

			if(i!=argc-1)
				strcat(c1," ");
		}

		printf("Executing command: %s\n",c1);
		system(c1);
		return 0;
	}

	gethostname(host,sizeof(host));

	while(1)
	{
		printf("<%d %s>",c,host);
		scanf(" %128[^\n]",cmd);

		strcpy(history[histCount%10],cmd);
		histSeq[histCount%10]=c;
		histCount++;

		char *save1;
		char *segment=strtok_r(cmd,";",&save1);

		while(segment!=NULL)
		{
			while(*segment==' ' || *segment=='\t')
				segment++;

			if(*segment=='\0')
			{
				segment=strtok_r(NULL,";",&save1);
				continue;
			}

			if(strchr(segment,'|'))
			{
				char *pipeCmd[10];
				int ncmd=0;
				int fd[2];
				int in=0;

				pipeCmd[ncmd]=strtok(segment,"|");

				while(pipeCmd[ncmd]!=NULL)
				{
					while(*pipeCmd[ncmd]==' ')
						pipeCmd[ncmd]++;

					ncmd++;
					pipeCmd[ncmd]=strtok(NULL,"|");
				}

				for(i=0;i<ncmd;i++)
				{
					pipe(fd);

					if(fork()==0)
					{
						dup2(in,0);

						if(i!=ncmd-1)
							dup2(fd[1],1);

						close(fd[0]);
						close(fd[1]);

						j=0;
						token[j]=strtok(pipeCmd[i]," \t");

						while(token[j]!=NULL)
						{
							j++;
							token[j]=strtok(NULL," \t");
						}

						execvp(token[0],token);
						perror("execvp");
						exit(1);
					}
					for(i=0;i<ncmd;i++)
					wait(NULL);

					close(fd[1]);
					in=fd[0];
				}

				close(in);
			}
			else
			{
				j=0;
				token[j]=strtok(segment," \t");

				while(token[j]!=NULL)
				{
					j++;
					token[j]=strtok(NULL," \t");
				}

				if(token[0]==NULL)
				{
					segment=strtok_r(NULL,";",&save1);
					continue;
				}

				if(strcmp(token[0],"quit")==0)
					return 0;

				else if(strcmp(token[0],"curpid")==0)
					printf("curpid=%d\n",getpid());

				else if(strcmp(token[0],"ppid")==0)
					printf("ppid=%d\n",getppid());

				else if(strcmp(token[0],"cd")==0)
				{
					if(token[1]==NULL)
						printf("cd: missing argument\n");
					else if(chdir(token[1])!=0)
						perror("cd failed");
					else
					{
						char buf[256];
						getcwd(buf,sizeof(buf));
						printf("Current directory: %s\n",buf);
					}
				}

				else if(strcmp(token[0],"hist")==0)
				{
					int start=histCount>10 ? histCount-10 : 0;

					for(i=start;i<histCount;i++)
						printf("%d %s\n",histSeq[i%10],history[i%10]);
				}

				else
				{       
					if(fork()==0)
					{
						int in=-1,out=-1;

						for(i=0;token[i]!=NULL;i++)
						{
							if(strcmp(token[i],"<")==0)
							{
								in=open(token[i+1],O_RDONLY);

								if(in<0)
								{
									perror("open");
									exit(1);
								}

								dup2(in,0);
								close(in);
								token[i]=NULL;
							}

							else if(strcmp(token[i],">")==0)
							{
								out=open(token[i+1],
										O_WRONLY|O_CREAT|O_TRUNC,
										0644);

								if(out<0)
								{
									perror("open");
									exit(1);
								}

								dup2(out,1);
								close(out);
								token[i]=NULL;
							}
						}

						execvp(token[0],token);
						perror("execvp");
						exit(1);
					}
					else
					{
						wait(NULL);
					}
				}

				c++;
				segment=strtok_r(NULL,";",&save1);
			}
		}
	}

	return 0;
}
