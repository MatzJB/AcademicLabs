/*
Matz Johansson B.

NOTE:

There will be an error if you do a parallel 'for' with two for-loops and don't
set the inner loop variable to private.

SYNOPSIS:

Reads a file with coordinates (arg[1]) and writes all distances to a file
(arg[2]). The matrix 'matrix' builds a frequency table with respect to distances
between each coordinate using OpenMP. Each thread handles its own column.
The (N_THREADS + 1)th column is reserved for the result (the sum of all rows).

DETAILS:

A hash function is used to store all distances.

The hash function relies on the following assumptions:
* Distances d ∈ [1, 50]
* Two cells c1 and c2, dist(c1, c2) >= 0.01

These two assumptions give us an array of distances (1:0.01:50) * 100, which results in:
100:5000 => [0, 4900]

The table that will contain all occurrences of distances must therefore have
4901 elements (including element 0)

*/

#define N_THREADS 2
#define TABELL_ELTS 4901
#define DEBUG 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <omp.h>

typedef struct
{
  float x, y, z;
} Cell;

int printed_rows = -1; // number of rows to print, -1 is all

Cell *cells;

int matrix[TABELL_ELTS][N_THREADS + 1];
int threads_workload[N_THREADS];

long int filesize(FILE *f);
void help(char *prgmname);
double csecond();

// Prints rows [0,n_rows] from matrix 'matrix'
void printmatrix(int n_rows)
{
  if (DEBUG)
  {
    int omit_row = 0;             // if all zeros, then omit the row
    int k = 0, i, j, printed = 0; // if not printed then matrix is empty

    if (n_rows < 0)
      n_rows = TABELL_ELTS;

    printf("\nMatrix: printing [0,%d] (%dx(%d+1))\n\n\t", n_rows, TABELL_ELTS, N_THREADS);

    for (i = 0; i < n_rows; i++)
    {
      omit_row = 1;
      for (k = 0; k < N_THREADS + 1; k++)
        if (matrix[i][k] != 0)
        {
          omit_row = 0;
          break;
        }

      for (j = 0; j < N_THREADS + 1; j++)
      {
        if (omit_row)
          break;
        else if (j == N_THREADS) // redundant?
          printf("| %d <%d>\n\t", matrix[i][j], i);
        else
          printed += printf("%d ", matrix[i][j]);
      }
    }

    if (!printed)
      printf("EMPTY MATRIX");
    else
      printf("\t\t----------");

    printf("\n");
  }
}

/*To write to a file with index, gives distance (rounded to 2
  significant digits)*/

/*[0,4900]->[1,50]*/
double index2dist(int k) { return 0.01 * k + 1; }

/* Returns number of rows in file. Assumes the file pointer is at start*/
long int filesize(FILE *f)
{
  long int fs = 0;

  char line[1025]; // including \0

  while (fgets(line, 1024, f) != NULL)
    fs++;

  rewind(f);
  return fs;
}

void help(char *prgmname)
{
  printf("%s usage:<infile> <outfile>\n\n", prgmname);
}

double csecond()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1.0E-6;
}

/* Do work: create frequency matrix and add it to our table (last column in the matrix)*/
void calc(int n_rows)
{
  int i, j, k, ind, i_am;

  if (DEBUG)
    for (i = 0; i < N_THREADS; i++)
      threads_workload[i] = 0;

  double t = 0, dx, dy, dz;

  t = csecond();
  {

#pragma omp parallel default(none) private(k, i_am, ind, dx, dy, dz) shared(matrix, n_rows, cells, threads_workload)
    {
      i_am = omp_get_thread_num();
#pragma omp for private(j) schedule(static, 1) // j not private 'by default'!

      for (i = 0; i < n_rows; i++)
      {
        for (j = i + 1; j < n_rows; j++)
        {
          dx = cells[i].x - cells[j].x;
          dy = cells[i].y - cells[j].y;
          dz = cells[i].z - cells[j].z;

          matrix[(int)(100 * (sqrt(dx * dx + dy * dy + dz * dz) - 1))][i_am] += 2;

          if (DEBUG)
            threads_workload[omp_get_thread_num()]++;
        }
      } // for
    } // pragma
  }
  t = csecond() - t;

  fprintf(stderr, "calc distance:%f sec.\n", t);

  if (DEBUG)
  {
    printmatrix(10); // first 10 rows of the matrix (if you want the
                     // whole matrix, preferably redirect it to a file)
  }

  t = 0; // reset

  t = csecond();
  {
    for (i_am = 0; i_am < N_THREADS; i_am++)
      for (k = i_am; k < i_am + TABELL_ELTS; k++)
      {
        ind = k % TABELL_ELTS;
        matrix[ind][N_THREADS] += matrix[ind][i_am];
      }
  }

  t = csecond() - t;

  if (DEBUG)
  {
    double sum = 0;

    for (i = 0; i < N_THREADS; i++)
      sum += threads_workload[i];

    printf("sum:%f\n", sum);

    for (i = 0; i < N_THREADS; i++)
      printf("Thread #%i workload: %f%%\n", i, 100. * (threads_workload[i] / sum));
  }
  fprintf(stderr, "merging the result:%f sec.\n", t);
}

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    help(argv[0]);
    exit(0);
  }

  //////////////////////////////////////////////////////////////////////
  // PREPROCESSING

  /* infile, outfile are pointers (file descriptors)*/
  FILE *infile, *outfile, *matlab_file;

  /* Open the file, named infile, using read-only mode. */
  if ((infile = fopen(argv[1], "r")) == NULL)
  {
    printf("Problems opening file '%s'.\n", argv[1]);
    exit(1);
  }

  /* For matlab*/
  if ((matlab_file = fopen("vis_cells.m", "w")) == NULL)
  {
    printf("Problems opening file 'matlab_test'.\n");
    exit(1);
  }

  // create the output file
  if ((outfile = fopen(argv[2], "w")) == NULL)
  {
    printf("Problems creating file '%s'.\n", argv[2]);
    exit(1);
  }

  int n_rows = filesize(infile);
  printf("n_rows = %d\n", n_rows);

  if ((cells = malloc(n_rows * sizeof(Cell))) == NULL)
  {
    printf("ERROR: alloc. of 'cells'\n");
    exit(0);
  }

  // Read all cells
  for (int i = 0; i < n_rows; i++)
    fscanf(infile, "%f %f %f", &cells[i].x, &cells[i].y, &cells[i].z);

  fclose(infile);

  //////////////////////////////////////////////////////////////////////
  // CALC STEP

  /* Set no. of threads dynamically. */
  omp_set_num_threads(N_THREADS);

  /* Max number of procs and threads. */
  printf("num_procs   = %d\n", omp_get_num_procs());
  printf("max_threads = %d\n", omp_get_max_threads());

  printmatrix(printed_rows);

  double t = 0;

  t = csecond();
  {
    calc(n_rows); // calculating the distances and puts them in tabell
  }
  t = csecond() - t;

  fprintf(stderr, "%f sec.\n", t);

  //////////////////////////////////////////////////////////////////////
  // POST PROCESSING

  // Calculate non-zero elements

  int n_uniq = 0;
  int sum_uniq = 0;

  for (int i = 0; i < TABELL_ELTS; i++)
    if (matrix[i][N_THREADS] != 0)
    {
      n_uniq++;
      sum_uniq += matrix[i][N_THREADS];
      fprintf(outfile, "%.2f\t%d\n", index2dist(i), matrix[i][N_THREADS]); // FEL f�rut
    }

  fprintf(outfile, "\nn_uniq\t%d\n", n_uniq);
  fprintf(outfile, "sum_uniq\t%d\n", sum_uniq);
  fclose(outfile);

  free(cells);

  return 0;
}

// old code below

/*

//writes to matlab file (just to see where the points are)
fprintf(matlab_file, "%%matrix from 'cells' gen. by Matz\n\n C = [\n");

for (int i=0; i<n_rows; i++)
  fprintf(matlab_file, "\t%f, %f, %f%s", cells[i].x, cells[i].y, cells[i].z, (i<n_rows-1)?";\n":"");

fprintf(matlab_file, "];\n\n");  //vill inte sluta med ;];
fprintf(matlab_file, "X = C(:,1);\n");  //vill inte sluta med ;];
fprintf(matlab_file, "Y = C(:,2);\n");
fprintf(matlab_file, "Z = C(:,3);\n\n");
fprintf(matlab_file, "plot3(X,Y,Z,'o');\n");

fclose(matlab_file);


//Test
if (0 == strcmp(argv[1],"cells2") && (sum_uniq != 4366010) )
fprintf(stderr,"ERROR!!!!\n");

*/
