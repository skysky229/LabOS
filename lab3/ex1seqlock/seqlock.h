#ifndef PTHREAD_H
#include <pthread.h>
#endif

typedef struct pthread_seqlock { /* TODO: implement the structure */
   int reader_count, write_wait, read_wait, turn;
   pthread_mutex_t mtx;
   pthread_cond_t wrlock;
   pthread_cond_t rdlock;
} pthread_seqlock_t;

static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   rw->reader_count = 0;
   rw->turn = 2;
   rw->read_wait = 0; rw->write_wait = 0;
   pthread_mutex_init(&rw->mtx, NULL);
   pthread_cond_init(&rw->wrlock, NULL);
   pthread_cond_init(&rw->rdlock, NULL);
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mtx);
   if (rw->turn == 1 || rw->reader_count > 1){
      rw->write_wait++;
      pthread_cond_wait(&rw->wrlock, &rw->mtx);
      rw->write_wait--;
   }
   rw->turn = 1;
   pthread_mutex_unlock(&rw->mtx);
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mtx);
   if (rw->write_wait > 0)
      pthread_cond_signal(&rw->wrlock);
   else {
      pthread_cond_signal(&rw->rdlock);
      rw->turn = 2;
   }
   pthread_mutex_unlock(&rw->mtx);
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mtx);
   if (rw->turn == 1){  // if currently a writer is writing
      rw->read_wait++;
      pthread_cond_wait(&rw->rdlock, &rw->mtx);
      rw->read_wait--;
   }
   rw->turn = 2;
   rw->reader_count += 1;
   pthread_cond_broadcast(&rw->rdlock);
   pthread_mutex_unlock(&rw->mtx);
   return 1;
}


static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mtx);
   rw->reader_count -= 1;
   if (rw->reader_count <= 1)
      pthread_cond_signal(&rw->wrlock);
   pthread_mutex_unlock(&rw->mtx);
   return 1;
}

