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
     int thread_count = atoi(argv[2]);
     int seednum = atoi(argv[3]);

     generate_data_array(arr, arraysize, seednum);

     pthread_t thread_id[thread_count];
     _range thread_range[thread_count];

     int length = arraysize/thread_count;
     for(int i=0;i<thread_count;i++){
          thread_range[i].start = i*length;
          thread_range[i].end = (i+1)*length - 1;
          // printf("%d %d \n", thread_range[i].start, thread_range[i].end);
          if (i == thread_count - 1)
               thread_range[i].end = arraysize;
          pthread_create(&thread_id[i], NULL, inter, (void *)&thread_range[i]);
     }

     for(int i=0;i<thread_count;i++){
          pthread_join(thread_id[i], NULL);
     }

     printf("Sum calculated by multiple threads: %lld\n", res);

     // long long sum = 0;
     // for(int i=0;i<arraysize;i++){
     //      printf("i-th element: %d\n", arr[i]);
     //      sum += arr[i];
     // }
     // printf("Sum calculated by for loops: %lld\n", sum);
}