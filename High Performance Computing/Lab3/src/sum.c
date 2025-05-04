
#include <pthread.h>
#include <stdio.h>

/* global shared variables */
#define VEC_LEN   400
#define N_THREADS   4
  double             a[VEC_LEN], b[VEC_LEN], sum; 
  pthread_mutex_t    mutexsum;

void* dotprod(void *arg) /* the slave */
{
  int     i, start, end, i_am, len;
  double  mysum;

  i_am = (int) arg;

  /* assume that N_THREADS divides VEC_LEN */
  len   = VEC_LEN / N_THREADS;
  start = i_am  * len;
  end   = start + len;

  mysum = 0.0; /* local sum */
  for (i = start; i < end; i++)
    mysum += a[i] * b[i];

  /* update global sum with local sum */
  pthread_mutex_lock(&mutexsum);
    sum += mysum;  /* critical section */
  pthread_mutex_unlock(&mutexsum);

  pthread_exit(NULL);  /* terminate the thread */
  return(NULL);        /* not executed         */
}


int main()
{
  pthread_t    thread_id[N_THREADS];
  int          i, ret;

  for (i = 0; i < VEC_LEN; i++) {
    a[i] =  1.0;  /* initialize */
    b[i] = a[i];
  }
  sum = 0.0;      /* global sum, NOTE declared global */

/* Initialize the mutex (mutual exclusion lock). */
  pthread_mutex_init(&mutexsum, NULL); 

/* Create threads to perform the dotproduct 
   NUll implies default properties.         */

  for(i = 0; i < N_THREADS; i++)
    if( ret = pthread_create(&thread_id[i], NULL,
                             dotprod, (void *) i)){
      printf ("Error in thread create\n");
      exit(1);
    }

/* Wait for the other threads. If the main thread exits
   all the slave threads will exit as well.             */

  for(i = 0; i < N_THREADS; i++)
     if( ret = pthread_join(thread_id[i], NULL) ) {
       printf ("Error in thread join %d \n", ret);
       exit(1);
     }

  printf ("sum = %f\n", sum);
  pthread_mutex_destroy(&mutexsum); 
  return 0;
}
