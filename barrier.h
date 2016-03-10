#include <pthread.h>
#include <stdbool.h>

typedef struct barrier_t {
  pthread_mutex_t mtx;
  pthread_cond_t cv;
  // Total number of threads
  unsigned n_threads;
  // Increasing or decreasing count
  unsigned count;
  // Usage number of the barrier
  unsigned int times_used;
} barrier_t;

/**
 * You may not modify the function signatures of the 3 functions in this header
 * file and you must implement them in your barrier.c.
 */
int barrier_destroy(barrier_t *barrier);
int barrier_init(barrier_t *barrier, unsigned num_threads);
int barrier_wait(barrier_t *barrier);
