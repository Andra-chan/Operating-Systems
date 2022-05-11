#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int a2b[2];
	int res = pipe(a2b);
	int p;
	if (res == -1)
	{
		perror("pipe(a2b) error: ");
		exit(EXIT_FAILURE);
	}
	int pid = fork();
	if (pid == -1)
	{
		perror("fork() error: ");
		exit(EXIT_FAILURE);
	}
	//B PROCESS
	if (pid == 0)
	{
		close(a2b[1]);
		read(a2b[0], &p, sizeof(int));
		while(1)
		{
			char str[20];
			read(a2b[0], str, sizeof(char)*20);
			if(strcmp(str, "stop")==0)
	                        break;
			if (p<=strlen(str))
				str[p-1] = 'X';
			printf("%s\n",str);
		}
		close(a2b[0]);
		exit(0);	
	}
	//A PROCESS
	close(a2b[0]);
	printf("p=");
	scanf("%d", &p);
	write(a2b[1], &p, sizeof(int));
	while(1)
	{
		char str[20];
		scanf("%s", str);
		write(a2b[1], str, 20*sizeof(char));
		if(strcmp(str, "stop")==0)
			break;	
	}
	close(a2b[1]);
	wait(0);
	return 0;	
}
