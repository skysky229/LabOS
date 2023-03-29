#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct thread_data {
     int lower;
     int upper;
     long long int res;
} thread_data;

void *calcSum(void *args){
     thread_data *data = (thread_data*) args;
     long long int sum = 0, lwb = data->lower, upb = data->upper;

     for(int i=lwb;i<=upb;i++)
          sum += i;
     data->res = sum;
     // printf("%d %d %d\n", sum, lwb, upb);

     pthread_exit(NULL); 
}

int main(int argc, char *argv[]){
     int n;
     n = atoi(argv[1]);

     pthread_t thread_id;
     thread_data thread_args;
     
     thread_args.lower = 1; thread_args.upper = n;
     pthread_create(&thread_id, NULL, calcSum, (void *)&thread_args);
     pthread_join(thread_id, NULL);
     long long int ans = thread_args.res;

     printf("Sum calculated by one thread (serial): %lld\n", ans);
}