#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h> 
#include <stdio.h>

double csecond()
{
  struct timeval  tv;
  gettimeofday(&tv, 0);
  //printf("%f\n", tv.tv_sec);
  return tv.tv_sec + tv.tv_usec * 1.0e-6;
}
