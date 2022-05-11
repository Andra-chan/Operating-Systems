/
// ex_model.c
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// maximum number of threads
//#define MAX_THR 100
#define MAX_SIZE 6000
#define MAX_NUM 200

int full = 0;            // gol: full = 0

// shared variable
int sum = 0;
int numbers[MAX_SIZE];

// lock object
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// thread start routine
void* f1(void* a)
{
     int N = *(int*)a;
     free(a);
     int i; 

     pthread_mutex_lock(&mtx);

     printf("Start reading the file...\n");
     int fd = open("file.bin", O_RDONLY);
     //FILE* fd = fopen("file.bin", "r");
     
          // srand(time(NULL));
     for(i=0;i<N;i++) {
          // numbers[i] = rand() % 255 + 1; // numere de la 0 la 255
          read(fd, &numbers[i], 1);
          // fscanf(fd, "%d", &numbers[i]);
     }

     full = 1;

     printf("Finished the reading.\n\n");    
     pthread_mutex_unlock(&mtx);

     close(fd);
     //fclose(fd);
     return NULL;
}

// thread start routine
void* f2(void* a)
{
     int k = *(int*)a;
     free(a);

     int min = k * MAX_NUM;
     int max = min + MAX_NUM;
     int i;
     for(i=min;i<max;i++) {

          pthread_mutex_lock(&mtx);
          while(full == 0) {
               pthread_cond_wait(&cv, &mtx);
          }

          sum += numbers[i];
          pthread_cond_signal(&cv);
          pthread_mutex_unlock(&mtx);
     }

     return NULL;
}


int main(int argc, char* argv[])
{
     // citesc valoarea lui N
     int N = 0;
     printf("Dati N [4000 - 6000]: ");
     scanf("%d", &N);

     // creez primul pthread
     pthread_t t1;
     int* k = (int*)malloc(sizeof(int));
     *k = N;
     pthread_create(&t1, NULL, f1, k);

     // calculez numarul necesar de pthreadu-uri
     int nthr = N/MAX_NUM;

     int i;
     pthread_t t2[nthr];
     for (i = 0; i < nthr; i++)
     {
          int* p = (int*)malloc(sizeof(int));
          *p = i;
          pthread_create(&t2[i], NULL, f2, p);
     }

     pthread_join(t1, NULL);

     for (i = 0; i < nthr; i++)
     {
          pthread_join(t2[i], NULL);
     }

     printf("Suma: %d\n", sum);

     return 0;
}

