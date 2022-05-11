#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int b2a[2];
	int res = pipe(b2a);
	int nr;
	int v[100];
	if (res == -1){
		perror("pipe(b2a) error: ");
		exit(EXIT_FAILURE); 
	}
	int pid=fork();
	if (pid == -1){
		perror("fork() error: ");
                exit(EXIT_FAILURE);
	}
	// A PROCESS - child
	if (pid==0)
	{
		close(b2a[0]);
		printf("Numere:");
		while(1)
		{	
			scanf("%d", &nr);
			write(b2a[1], &nr, sizeof(int)); 
			if (nr==0)
				break;
		}
		close(b2a[1]);
		exit(0);
	}
	// B PROCESS - parent
	int n=0;
	close(b2a[1]);
	while(1)
	{
		read(b2a[0], &nr, sizeof(int));
		if (nr==0)
			break;
		v[n]=nr;
		n++;
	}	
	close(b2a[1]);
	wait(0);
	int i,j;
	for (i=0;i<n-1;i++)
		for (j=i+1;j<n;j++)
			if (v[i]>v[j])
			{
				int aux = v[i];
				v[i] = v[j];
				v[j] = aux;	
			}
	printf("Sir sortat:");
	for (i=0;i<n;i++)
		printf("%d ", v[i]);	
	printf("\n");
	return 0;

}
