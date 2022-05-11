#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


pthread_t* t;
pthread_mutex_t* mtx;
typedef struct{
	char* nr;
	int index;
}my_struct;
int N;

int n;
int gval=0;
int ok=0;

void* f(void* params){
	my_struct* val=(my_struct*)params;
	int i=val->index;
	int valoare=atoi(val->nr);
	while(1){
		pthread_mutex_lock(&mtx[i]);
		gval+=valoare;
		printf("Thread %d - gval: %d\n",i,gval);
		int j;
		if(gval%n==0){
			printf("Se schimba ordinea!\n");
			ok=!ok;
			n++;
		}
		if(ok!=0){
			j=i-1;
			if(j<0)
				j=N-1;
		}
		else{
			int j=i+1;
			j=j%N;
		}
		
		pthread_mutex_unlock(&mtx[j]);
	}
	return NULL;
}

int main(int argc, char** argv){
	N=argc-1;
	my_struct* ms=(my_struct*)malloc(sizeof(my_struct)*N);
	n=atoi(argv[1]);
	t=(pthread_t*)malloc(sizeof(pthread_t)*N);
	mtx=(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*N);
	int i;
	for(i=0;i<N;i++){
		pthread_mutex_init(&mtx[i],NULL);
		pthread_mutex_lock(&mtx[i]);
		ms[i].nr=argv[i+1];
		ms[i].index=i;
		pthread_create(&t[i],NULL,f,(void*)&ms[i]);
	}
	pthread_mutex_unlock(&mtx[0]);
	for(i=0;i<N;i++){
		pthread_join(t[i],NULL);
	}
	free(t);
	free(ms);
	free(mtx);
	return 0;
}