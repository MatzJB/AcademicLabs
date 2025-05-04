#include "lab2_standard.h"

/* this is inspired by programming language MPD and the course concurrent programming */

typedef int *semaphore;

void p(semaphore s);
void v(semaphore s);

semaphore create_semaphore(int bin); // binary semaphore
