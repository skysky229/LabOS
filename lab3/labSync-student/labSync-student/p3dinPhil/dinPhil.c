#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

pthread_mutex_t mtx;
pthread_cond_t chopstick[N];
enum { THINKING, HUNGRY, EATING} state [N];

void *philosopher(void*);
void eat(int);
void think(int);
void test(int i){
   if ((state[(i + (N-1)) % N] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING) ) {
      state[i] = EATING;
      pthread_cond_signal(&chopstick[i % N]);
   }
}

void pickup(int i){
   state[i] = HUNGRY;
   test(i);
   if (state[i] != EATING){
      pthread_cond_wait(&chopstick[i % N], &mtx);
   }
}

void putdown(int i){ 
   state[i] = THINKING;
   test((i+(N-1)) % N);
   test((i+1) % N);
}

int main()
{
   int i, a[N];
   pthread_t tid[N];

   pthread_mutex_init(&mtx, NULL);
   pthread_mutex_lock(&mtx);
   for (i = 0; i < N; i++){
	   pthread_cond_init(&chopstick[i], NULL);
      pthread_cond_broadcast(&chopstick[i]);
   }
   pthread_mutex_unlock(&mtx);

   for (i = 0; i < 5; i++){
      state[i] = THINKING;
   }

   for (i = 0; i < 5; i++)
   {
      a[i] = i;
      pthread_create(&tid[i], NULL, philosopher, (void*) &a[i]);
   }

   for (i = 0; i < 5; i++)
      pthread_join(tid[i], NULL);
}

void *philosopher(void *num){
   int phil = *(int*) num;
   printf("Philosopher %d has entered room\n", phil);
	  
   while (1)
   {
      //pthread_cond_wait(&chopstick[phil], &mtx);
      //pthread_cond_wait(&chopstick[(phil + 1) % N], &mtx);
      pickup(phil);
      printf("Philosopher %d takes fork %d and %d\n",
	          phil, phil, (phil + 1) % N);
			  
      eat(phil);
      sleep(2);

       printf("Philosopher %d puts fork %d and %d down\n",
	          phil, (phil + 1) % N, phil);
      // pthread_cond_signal(&chopstick[phil]);
      // pthread_cond_signal(&chopstick[(phil + 1) % N]);
      putdown(phil);

	  think(phil);
	  sleep(1);
   }
}

void eat(int phil)
{
   printf("Philosopher %d is eating\n", phil);
}

void think(int phil)
{
   printf("Philosopher %d is thinking\n", phil);
}