#define _GNU_SOURCE

#include "barrier.h"
#include <stdlib.h>

/* I don't like checking for errors so I leave the error untouched*/

int barrier_destroy(barrier_t *t) {
  int error = 0;
  pthread_mutex_lock(&t->mtx);
  // If the barrier is busy, wait
  if (t->count != 0){
    pthread_cond_wait(&t->cv,&t->mtx);
  }
  pthread_mutex_unlock(&t->mtx);
  pthread_mutex_destroy(&t->mtx);
  pthread_cond_destroy(&t->cv);
  return error;
}

int barrier_init(barrier_t *t, unsigned n) {
  int error = 0;
  pthread_mutex_init(&t->mtx,NULL);
  pthread_cond_init(&t->cv,NULL);
  t->n_threads = n;
  t->count = 0;
  t->times_used = 0;
  return error;
}

int barrier_wait(barrier_t *t) { 
  unsigned cycle;
  pthread_mutex_lock(&t->mtx);
  cycle = t->times_used;
  if (++t->count == t->n_threads){
    t->times_used+=1;
    t->count = 0;
    pthread_cond_broadcast(&t->cv);
  }
  else{
    while (cycle == t->times_used){
      pthread_cond_wait(&t->cv,&t->mtx);
    }
  }
  pthread_mutex_unlock(&t->mtx);
  return 0; 
}
