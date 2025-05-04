#include "frequence.h"


void dispRateTable(RateTable rates, int size)
{
  if (DETAIL!=nothing)
    {
      printf("[");

      for (int i=1; i<=size; i++)
	{
	  if (i%size!=1 && size!=1)
	    printf(",");
	  printf("%lu", rates[i-1]);//cast to int
	}

      printf("]\n ");
    }
}

/*
  Counts the collected rate of occurance for each unique character in a file
  and puts them in an array
*/

void collectRate(FILE *thefile, RateTable dist, unsigned long * filesize)
{

  int counter = * filesize;

  if (thefile == NULL)
    disp("collectRate()", "the file does not exist", "check if you type the right filename", extreme, DETAIL);

  int j, c;

  setDefaultRate(dist, TABLE_SIZE);

  while( (c = getc(thefile)) != EOF)
    {

      if (c >= 0 && c <= TABLE_SIZE)
	{
	  j = c;
	  assert(0 <= j && j < TABLE_SIZE);//check array index
	  dist[j]++;
	}
      counter++;
    }

  if (counter == 0)
    disp("collectRate()", "No calculation performed","make sure the file contain data", did_you_know, DETAIL);

  *filesize = counter;

}


//resets all elements in a RateTable
void setDefaultRate(RateTable f, int nrOfElts)
{
  for(int i=0; i<nrOfElts;i++)
    f[i]=0;
}
