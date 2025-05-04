/*
   author: Matz Johansson
   news:
   disp invoked, exits if semaphore creation error
*/

#include "lab2_standard.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>

/*
  Edsgar Dijkstras notation, checked with Koen about the dutch p and v.
*/

void p(semaphore s) // wait
{
  int junk;

  if (read(s[0], &junk, 1) <= 0)
    disp("semaphore p", "\"Proberen\" <lock> failed", "check creation of semaphore", extreme, STYLE);
  // else
  // disp("p","lock","",did_you_know,STYLE);
}

void v(semaphore s) // signal
{

  if (write(s[1], "x", 1) <= 0)
    disp("semaphore v", "\"Verhogen\" <release> failed", "check creation of semaphore", extreme, STYLE);
  // else
  // disp("v","release","",did_you_know,STYLE);
}

semaphore create_semaphore(int init)
{
  disp("create_semaphore", "semaphore created", "set to 1", did_you_know, STYLE);

  int *sema;

  sema = calloc(2, sizeof(int)); /*permanent lagring*/
  pipe(sema);

  if (init == 1) /*if 1 then init with write else no writing*/
    v(sema);
  else
    disp("create_semaphore", "semaphore error", "", extreme, STYLE); // skall jag avsluta, hmmm jo!

  return sema;
}
