#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_llong res = 0;
int arr[300000005];

typedef struct _range{
     int start;
     int end;
} _range;

int generate_data_array(int* buf, int arraysize, int seednum){
     srand(seednum);
     for(int i=0;i<arraysize;i++){
          buf[i] = rand();
     }
     return 1;
}

void* sum_worker(_range idx_range){
     int lower = idx_range.start;
     int upper = idx_range.end;
     for(int i=lower;i<=upper;i++)
          res += arr[i];
     // printf("%d %d %lld \n", lower, upper, res);
}

void* inter(void* args){
     _range *idx_range = (_range*) args;
     sum_worker(*idx_range);
}

int main(int argc, char* argv[]){
     int arraysize = atoi(argv[1]);
     int seednum = atoi(argv[2]);

     generate_data_array(arr, arraysize, seednum);

     pthread_t thread_id;
     _range thread_range;
     thread_range.start = 0;
     thread_range.end = arraysize - 1;
     pthread_create(&thread_id, NULL, inter, (void *)&thread_range);
     pthread_join(thread_id, NULL);

     printf("Sum calculated by single threads: %lld\n", res);
}