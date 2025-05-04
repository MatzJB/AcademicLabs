/* This simple program shows how the graphics package can be used. 
   It will show the available colors in a set of eight bands.
   row % 50 is the remainder when dividing row with 50.
*/

#include "defs.h"      /* Some typedefs and prototypes.      */
#define WIDTH  400     /* Useful, since they will be needed  */
#define HEIGHT 400     /* in several places in your program. */

int main()
{
  int   row, col, color, line[WIDTH];    /* Allocate storage for line.   */

  OpenWindow(WIDTH, HEIGHT);             /* The first graphics call.     */

  color = -1;                            /* -1 is an illegal color.
                                            Note color++ below.          */

  for( row = 0; row < HEIGHT; row++ ) {  /* One row at the time.         */
    if ( row % 50 == 0 ) color++;        /* Make colored bands.          */
    for (col = 0; col < WIDTH; col++)    /* For all pixels in the row.   */
      line[col] = color;                 /* Store color in line.         */

    DrawLine(row, line);                 /* Draw line as row number row. */
  }

  FlushWindow();  /* To make sure that everything has been plotted. */
  CloseWindow();  /* Wait for user to type q in window.             */

  return 0;
}
