#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int numar_divizori(int x)
{
	int i, nr=0;
	for (i=1;i<=x;i++)
		if (x%i==0)
			nr++;
	return nr;
}

int main(int argc, char* argv[]){
	int p2c[2];
	int res = pipe(p2c);
	int nr;
	if (res == -1){
		perror("pipe(p2c) error: ");
		exit(EXIT_FAILURE); 
	}
	int c2p[2];
        pipe(c2p);
        if (res == -1){
                perror("pipe(b2a) error: ");
                exit(EXIT_FAILURE);
        }

	int pid=fork();
	if (pid == -1){
		perror("fork() error: ");
                exit(EXIT_FAILURE);
	}
	// CHILD PROCESS
	if (pid==0)
	{
		close(c2p[0]);
		close(p2c[1]);
		while(1)
		{
			read(p2c[0], &nr, sizeof(int));
			if (nr==0)
				break;
			int nr_d = numar_divizori(nr);
			write(c2p[1], &nr_d, sizeof(int));
			int i;	
			for (i=1;i<=nr;i++)
				if (nr%i==0)
					write(c2p[1], &i, sizeof(int));			
		}
		close(c2p[1]);
		close(p2c[0]);
		exit(0);	
	}
	// PARENT PROCESS
	close(p2c[0]);
	close(c2p[1]);
	printf("Numere:");
	while (1)
	{
		scanf("%d", &nr);
		write(p2c[1], &nr, sizeof(int));
		if (nr==0)
			break;
		int nr_d;
		read(c2p[0], &nr_d, sizeof(int));
		int i;
		printf("Divizori lui %d:", nr);
		for (i=1;i<=nr_d;i++)
		{
			int d;
			read(c2p[0], &d, sizeof(int));
			printf("%d ", d);
		}
		printf("\n");
	}
	close(c2p[0]);
        close(p2c[1]);
	wait(0);

	return 0;
}
