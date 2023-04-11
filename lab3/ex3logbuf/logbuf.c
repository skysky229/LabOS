#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6
#define MAX_LOOPS 30

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];

int count;
void flushlog();
pthread_mutex_t lock;
pthread_cond_t condition;

struct _args {
   unsigned int interval;
};

void *wrlog(void *data)
{
   char str[MAX_LOG_LENGTH];
   int id = *(int*) data;
   usleep(20);

   sprintf(str, "%d", id);
   pthread_mutex_lock(&lock);
   if (count == MAX_BUFFER_SLOT)
      pthread_cond_wait(&condition, &lock);
   strcpy(logbuf[count], str);
   count = (count > MAX_BUFFER_SLOT)? 0 :(count + 1); /* Reset count to 0 when it reaches MAX_BUFFER_SLOT */
   pthread_mutex_unlock(&lock);

   //printf("wrlog(): %d \n", id);

   return 0;
}

void flushlog()
{
   int i;
   char nullval[MAX_LOG_LENGTH];

   pthread_mutex_lock(&lock);
   // printf("flushlog()\n");
   sprintf(nullval, "%d", -1);
   for (i = 0; i < count; i++)
   {
      printf("Slot  %i: %s\n", i, logbuf[i]);
      strcpy(logbuf[i], nullval);
   }

   fflush(stdout);

   /* Reset count to 0 */
   count = 0;

   for(int i=0;i<MAX_BUFFER_SLOT;i++)
      pthread_cond_signal(&condition);
   pthread_mutex_unlock(&lock);

   return;
}

void *timer_start(void *args)
{
   while (1)
   {
      flushlog();
      /*Waiting until the next timeout */
      usleep(((struct _args *) args)->interval);
   }
}

int main()
{
   int i;
   count = 0;
   pthread_t tid[MAX_LOOPS];
   pthread_t lgrid;
   int id[MAX_LOOPS];

   struct _args args;
   args.interval = 500e3;
   /*500 msec ~ 500 * 1000 usec */

   pthread_mutex_init(&lock, NULL); 
   pthread_cond_init(&condition, NULL);

   /*Setup periodically invoke flushlog() */
   pthread_create(&lgrid, NULL, &timer_start, (void*) &args);

   /*Asynchronous invoke task writelog */
   for (i = 0; i < MAX_LOOPS; i++)
   {
      id[i] = i;
      pthread_create(&tid[i], NULL, wrlog, (void*) &id[i]);
   }

   for (i = 0; i < MAX_LOOPS; i++)
      pthread_join(tid[i], NULL);

   pthread_mutex_destroy(&lock); /* Destroy the mutex */
   pthread_cond_destroy(&condition);

   sleep(5);

   return 0;
}