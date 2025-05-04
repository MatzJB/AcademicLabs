/*
FL 148 about MEX

Working with MEX:
mex -f ./mexopts.sh lab2_7.c uppg7_blas/*.f

You need to know the upper and lower bandwidth, the matrix, and the vector.
*/

// spdiags(ones(n, 2*width+1), floor(-width:width), n, n);

/*
  y = alpha * A * x + beta * y
  y = 1 * A * x

  lab2_7(A, super, sub, x)
  lab2_7(A, ku, kl, x)
*/

#include <math.h>
#include <mex.h>

extern void dgbmv_(char *trans, int *m, int *n, int *kl, int *ku, double *alpha, double *A, int *LDA, double *x, int *incx, double *beta, double *y, int *incy);
#define in(a, i, j) *(a + i * n + j)

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  char trans = 'N';
  char ture[1000];
  int m, n, kl, ku, LDA, incx = 1, incy = 1;
  double *A, *x, beta = 0.0, *y, alpha = 1.0;

  double *size;

  //  if (nlhs!=1)
  // mexWarnMsgTxt("Wrong number of returning values, it returns a vector");

  if (nrhs != 4)
    mexErrMsgTxt("usage:lab2_7(matrix A, int super, int sub, vector x)");

  A = mxGetPr(prhs[0]);
  m = mxGetM(prhs[0]);
  n = mxGetN(prhs[0]);

  sprintf(ture, "A[0][0]=%f A[1][0]=%f A[2][0]=%f \n"
                "A[0][1]=%f A[1][1]=%f  A[2][1]=%f\n"
                " A[0][2]=%f A[1][2]=%f A[2][2]=%f\n ",
          in(A, 0, 0), in(A, 1, 0), in(A, 2, 0),
          in(A, 0, 1), in(A, 1, 1), in(A, 2, 1),
          in(A, 0, 2), in(A, 1, 2), in(A, 2, 2));

  mexWarnMsgTxt(ture);

  /* size = mxGetPr(plhs[1]);
   * size = mxGetM(prhs[1]);
   */
  if (mxGetM(prhs[3]) != n)
    mexErrMsgTxt("the vector x has the wrong size");

  LDA = n;

  ku = mxGetN(prhs[1]); // super
  kl = mxGetN(prhs[2]); // sub
  x = mxGetPr(prhs[3]);

  // y is overwritten
  plhs[0] = mxCreateDoubleMatrix(n, 1, mxREAL); // result vector
  y = mxGetPr(plhs[0]);

  dgbmv_(&trans, &m, &n, &kl, &ku, &alpha, A, &LDA, x, &incx, &beta, y, &incy);

  /*
    sprintf(ture,"y= %f %f %f\n ", y[0], y[1], y[2]);
  mexWarnMsgTxt(ture);
  */

  return;
}
