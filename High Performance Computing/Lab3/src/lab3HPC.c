/*
Matz Johansson B.
Assumption: The roots are given and stored in a global 'Complex array'.


Synopsis:
I let the threads render all the rows. Once all rows are drawn, the threads shut down. After that, the roots are drawn (to avoid drawing roots over the fractal).

A while loop in main handles repeated button presses. When a button is pressed, the range, etc., is recalculated, and <n_threads> threads are started to render the new image.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "defs.h"

#define N_THREADS 5
#define ROWS 600
#define COLS 600
#define USAGE printf("%s usage: [-verbose] \n", argv[0])

pthread_mutex_t mutexplot;
pthread_mutex_t mutexrow;

// #define P(x) ;//pthread_mutex_lock(x);
// #define V(x) ;//pthread_mutex_unlock(x);

// Complex roots[3] = {{1, 0}, {-0.5, -0.8660},{-0.5, 0.8660}};
Complex roots[3] = {{1, 0}, {-0.5, -0.8660}, {-0.5, 0.8660}};
// int n_roots=3;

Complex range[2] = {{-1.5, -1.5}, {1.5, 1.5}}; // range[0]<range[1]
int verbose = 0;                               // 1 or 0
int current_row_n = 0;

Complex f(Complex x);
int NewtonColor(Complex guess);
void RowColor(int row, int *arr);
void *Render(void *arg);
Complex graphics2mathC(int row, int col);
void math2graphicsC(Complex coord, int koord[]);
void ThreadMaster();
void DrawRoots();

// This is drawn after all threads are finished (see ThreadMaster)
void DrawRoots()
{
  int koord[2];

  for (int j = 0; j < 3; j++)
  {
    math2graphicsC(roots[j], koord);

    if (koord[0] > 0 && koord[0] < COLS && koord[1] > 0 && koord[1] < ROWS)
    {
      DrawCircle(koord[0], koord[1], 5, 0);
      DrawCircle(koord[0], koord[1], 3, 1);
    }
  }

  free(koord);
}

// x^3-1
// x^5-1
//(x^5-1)/5x^4
//(-1+x^3)/5x^4
Complex f(Complex x)
{
  Complex xpow2 = Mul(x, x);
  return Sub(x, Div(Adds(-1, Mul(xpow2, x)), Muls(3, xpow2)));
}

// Finds root and gives a color to a complex number
int NewtonColor(Complex guess)
{
  Complex Xn, Xn_1 = {5., 5.}; // Xn_1 far away
  double delta = 1E-3;
  int i;

  Xn = guess;

  // Iterate until diff is small enough
  // compare with 3 know roots
  while (Cabs(Xn, Xn_1) >= delta)
  {
    Xn_1 = Xn; // replace old value
    Xn = f(Xn_1);
  }

  int foundit = -1;

  for (i = 0; i < 3; i++)
    if (Cabs(Xn, roots[i]) <= delta)
      foundit = i; // i<-[0,3[, i==-1 if no root was found

  if (foundit == -1)
  {
    printf("ERROR: No root found\n"); // could not find root, decrease delta
    exit(1);
  }

  return foundit + 2; // color in graphics is [2, 3, 4] [R, G, B] respectively
}

// Translates from coordinate to complex number
Complex graphics2mathC(int col, int row)
{
  Complex span = Sub(range[1], range[0]); // the span of the set

  double guess_r = col / (COLS - 1.0) * span.r + (range[0]).r;
  double guess_i = row / (ROWS - 1.0) * span.i + (range[0]).i;

  return Double2Complex(guess_r, guess_i);
}

// Translates from complex number to coordinate
void math2graphicsC(Complex coord, int koord[])
{
  Complex span = Sub(range[1], range[0]); // the span of the set

  koord[0] = ((coord.r - (range[0]).r) / span.r) * (COLS - 1);
  koord[1] = ((coord.i - (range[0]).i) / span.i) * (ROWS - 1);
}

/*
  Coloring using Newtons method
  Returns an array with integers
*/
void RowColor(int row, int *arr)
{
  Complex guess;

  for (int x = 0; x < COLS; x++) //[0,899] if COLS==900
  {
    guess = graphics2mathC(x, row);
    arr[x] = NewtonColor(guess);
  }
}

void *Render(void *arg)
{
  int my_row = 0;
  int *colors_array; // array of colors (<-[0,7])

  if ((colors_array = malloc(COLS * sizeof(int))) == NULL)
  {
    printf("ERROR:: [Render] Could not malloc colors_array.\n");
    exit(1);
  }

  while (my_row <= ROWS)
  {
    pthread_mutex_lock(&mutexrow);
    {
      my_row = current_row_n;
      current_row_n++;
    }
    pthread_mutex_unlock(&mutexrow);

    RowColor(my_row, colors_array);

    pthread_mutex_lock(&mutexplot);
    {
      DrawLine(my_row, colors_array);
    }
    pthread_mutex_unlock(&mutexplot);
  }

  free(colors_array); //*
  pthread_exit(NULL); /* terminate the thread */
  return (NULL);      /* not executed         */
}

// deals with the threads, wakes them up, and kills them, easier to handle them here
void ThreadMaster()
{
  int i, ret;

  pthread_t thread_id[N_THREADS];

  if (verbose)
    printf("\tStarting threads\n");

  /* Initialize the mutex (mutual exclusion lock). */
  pthread_mutex_init(&mutexplot, NULL);
  pthread_mutex_init(&mutexrow, NULL);

  for (i = 0; i < N_THREADS; i++)
    if ((ret = pthread_create(&thread_id[i], NULL, Render, (void *)i)))
    {
      printf("Error::[ThreadMaster:pthread_create], after %d threads has been created\n", i + 1);
      exit(1);
    }

  if (verbose)
    printf("\tTerminating threads...\n");

  for (i = 0; i < N_THREADS; i++)
    if ((ret = pthread_join(thread_id[i], NULL)))
    {
      printf("Error:: [ThreadMaster:pthread_join] returned %d \n", ret);
      exit(1);
    }

  pthread_mutex_destroy(&mutexplot);
  pthread_mutex_destroy(&mutexrow);

  DrawRoots();
  current_row_n = 0; // rest since we want to redraw from beginning
}

int main(int argc, char **argv)
{
  int btn, xco, yco; // mouse buttons and coordinates
  Complex span, pressed;
  int nr_zoom = 0; // number of times we zoomed
  ///////////////////////////////////

  if (argc == 2) //[-verbose]
    if (strcmp("-verbose", argv[1]) == 0)
      verbose = 1;
    else
    {
      printf("%s:unrecognized option '%s', did you mean '-verbose'?\n", argv[0], argv[1]);
      exit(1);
    }

  if (argc != 1 && argc != 2) // nothing is right
  {
    printf("syntax error, too many arguments\n");
    USAGE;
    exit(1);
  }

  ///////////////////////////////////

  printf("HPC LAB 3 Newton Fractal\n");
  printf("by Matz Johansson B.\n\n");
  USAGE;

  printf("Instructions:\nClicking Mouse button; \n\t1 Zooms in under cursor\n\t2 and then q exits program\n\t3 Zooms out\n");

  printf("Warning: On some systems creating ~304 threads makes the program crash\n on Chalmers computers.\n");
  printf("Warning: If 'No Root Found' appears, just set the delta variable to a smaller value\n");

  if (N_THREADS > ROWS)
    if (verbose)
      printf("Warning: Number of threads > number of rows!\n");

  if (verbose)
    printf("window size: %dx%d\n", COLS, ROWS);
  if (verbose)
    printf("number of threads to be created: %d\n", N_THREADS);

  OpenWindow(COLS, ROWS);

  while (1)
  {
    ThreadMaster(); // Start the threads and render
    FlushWindow();  /* To make sure that everything has been plotted. */

    WaitForButton(&xco, &yco, &btn);
    DrawCircle(xco, yco, 5, 1);

    if (verbose)
      printf("You clicked:(%d, %d)\n", xco, yco);

    span = Sub(range[1], range[0]); // the span of the set
    pressed = graphics2mathC(xco, yco);

    if (btn == 1)
    {
      if (verbose)
        printf("Zooming in...\n");
      range[0] = Double2Complex(pressed.r - span.r / 4., (pressed.i) - span.i / 4.);
      range[1] = Double2Complex(pressed.r + span.r / 4., (pressed.i) + span.i / 4.);
      nr_zoom++;
    }
    else if (btn == 2) // panning
    {
      //	  printf("Panning...\n");
      // range[0] = Double2Complex(pressed.r-span.r/2., (pressed.i)-span.i/2.);
      // range[1] = Double2Complex(pressed.r+span.r/2., (pressed.i)+span.i/2.);
      printf("...exiting\n");

      break;
    }
    else if (btn == 3)
    {
      if (verbose)
        printf("zooming out...\n");
      range[0] = Double2Complex(pressed.r - span.r, (pressed.i) - span.i);
      range[1] = Double2Complex(pressed.r + span.r, (pressed.i) + span.i);
      nr_zoom--;
    }
    if (verbose)
    {
      printf("\tzoomed in :%d times\n", nr_zoom);
      printf("New span:");
      PrintC(span);
      printf("New range:\n");
      PrintC(range[0]);
      PrintC(range[1]);
    }
    if (nr_zoom > 51)
    {
      printf("Warning: Now you probably won't see anything but artifacts (i.e large pixels), zooming out from this point will probably mess up the range\n");
    }
  }

  if (verbose)
    printf("closing window\n");

  CloseWindow();
  return 0;
}
