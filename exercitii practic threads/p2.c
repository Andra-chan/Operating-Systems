/*Scrieti un program c care primeste ca si argumente in linie de comanda
oricate numere intregi. Pentru fiecare argument primit programul va crea
cate doua thread-uri A si B. Fiecare thread A va primi ca si parametru unul
din numerele date ca argument in linia de comanda. Thread-urile A vor adauga
la o variabila globala valoarea primita ca parametru, de un numar de ori
egal cu valoarea argumentului. Dupa terminarea executei unui thread A se va
porni executai uniu thread B. Thread-urile B vor afisa pe ecran divizorii
variabilei globale mentionate anterior, in timpul afisarii divizorilor
variabilei globale thread-urile A nu vor modifica continutul acesteia.
Toate thread-urile A isi vor incepe executia simultan, adica nu va incepe
fiecare la momentul la care a fost creat.
Se vor afisa pe ecran mesaje si date care sa faca posibila urmarirea
desfasurarii rularii thread-urilor.
Accesul la variabilele globale trebuie sincronizat in cel mai eficient mod
posibil.
Programul trebuie compilat folosind utilitarul gcc cu obtiunile -Wall si -g
fara a rezulta nici un avertisment sau o eroare de sintaxa.
Programul trebuie sa ruleze fara memory leak sau erori de context.
Se vor face toate validarile necesare.
In cazul alocarilor dinamice nu se va aloca mai mult spatiu decat este
necesar.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t* t;
pthread_mutex_t mtx;
pthread_barrier_t bar;

typedef struct{
char* nr;
int index;
}my_struct;

int val=0;

void* f2(void* a);

void* f1(void* a)
{
    pthread_barrier_wait(&bar);
    my_struct* my_param=(my_struct*)a;
    int nr = atoi(my_param->nr);
    int index=my_param->index;
    int i;

    for(i=0;i<nr;i++){
	pthread_mutex_lock(&mtx);
	val+=nr;
	if(i == nr - 1)
	{
		pthread_create(&t[2 * index + 1], NULL, f2, a);
		pthread_join(t[2 * index + 1], NULL);
	}
	pthread_mutex_unlock(&mtx);
    }

    return NULL;
}

void* f2(void* a)
{
	int i = ((my_struct*)a)->index;
	
	int d;
	printf("Divizorii pt. %d sunt:\n", val);
	for(d = 1; d <= val / 2; d++)
		if(val % d == 0)
			printf("%d ", d);
	printf("%d", val);
	printf("\n");

	return NULL;
}

int main(int argc, char** argv)
{
    
    int N=argc-1;
    if(argc < 2)
    {
        perror("Nu s-a furnizat niciun argument la linia de comanda!");
        exit(1);
    }
    my_struct* my_val=(my_struct*)malloc(sizeof(my_struct)*N);
    t=(pthread_t*)malloc(sizeof(pthread_t)*N*2);
    
    pthread_barrier_init(&bar,NULL,N);
    
    int i;

    for(i=0;i<N;i++){
        my_val[i].nr=argv[i+1];
        my_val[i].index=i;
	pthread_create(&t[2*i],NULL,f1,&my_val[i]);
	
    }

	printf("Aici 3\n");

    for(i=0;i<N;i++){
    	pthread_join(t[2*i],NULL);
    }

  pthread_barrier_destroy(&bar);
    
pthread_mutex_destroy(&mtx);

    free(t);

    return 0;
}
