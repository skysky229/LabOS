#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   //khai bao mutex

typedef struct thread_data {
     int lower;
     int upper;
     long long int res;
} thread_data;

void *calcSum(void *args){
     // pthread_mutex_lock(&mutex);

     thread_data *data = (thread_data*) args;
     long long int sum = 0, lwb = data->lower, upb = data->upper;

     for(int i=lwb;i<=upb;i++)
          sum += i;
     data->res = sum;
     // printf("%d %d %d\n", sum, lwb, upb);

     // pthread_mutex_unlock(&mutex);
     pthread_exit(NULL); 
}

int main(int argc, char *argv[]){
     int n, num_thread;
     num_thread = atoi(argv[1]); n = atoi(argv[2]);

     pthread_t thread_id [num_thread];
     thread_data thread_args[num_thread];
     long long int ans = 0;
     
     int length = n/num_thread;
     for(int i=0;i<num_thread;i++){
          thread_args[i].lower = i*length + 1;
          thread_args[i].upper = (i+1)*length;
          if (i == num_thread - 1)
               thread_args[i].upper = n;
          pthread_create(&thread_id[i], NULL, calcSum, (void *)&thread_args[i]);
     }

     for(int i=0;i<num_thread;i++){
          pthread_join(thread_id[i], NULL);
          ans += thread_args[i].res;
     }

     printf("Sum calculated by multiple threads: %lld\n", ans);
}