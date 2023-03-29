#ifndef PTHREAD_H
#include <pthread.h>
#endif

typedef struct pthread_seqlock { /* TODO: implement the structure */
   int reader_count;
   pthread_mutex_t mtx;
   pthread_mutex_t reader_mtx;
   pthread_cond_t lock;
} pthread_seqlock_t;

static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   reader_count = 0;
   turn = 2;
   pthread_mutex_init(&mtx);
   pthread_mutex_init(&reader_mtx);
   pthread_cond_init(&lock);
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&mtx);
   while (turn == 1 || reader_count > 1)
      pthread_cond_wait(&lock, &mtx);
   pthread_mutex_unlock(&mtx);
   turn = 1;
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&mtx);
   turn = 2;
   pthread_cond_signal(&lock);
   pthread_mutex_unlock(&mtx);
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   if (turn == 2) 
   
}


static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */

   return 0;
}

