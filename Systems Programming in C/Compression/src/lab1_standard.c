#include "lab1_standard.h"

void bit_print(int a, int nrofbits) //copied from the book
{
  int i;
  int n = nrofbits;
  int mask = 1<<(n-1);

  for(i=1; i<=n; ++i)
    {
      putchar(((a & mask) == 0)? '0':'1');
      a<<=1;
    }
}

void disp(char * where, char * what, char * precaution, CautionLevel caution, MessageDetail detail)
{
  char * caut[] = {"Extreme","*High*","Important!","~Serious~","Warning","Observe","Did you know!"};

  //if (DETAIL == all)
   printf("\n");
  
  if (strcmp(precaution, "") == 0)
    precaution="none";
  
  if (detail == verbose || detail == all)
    printf("%s>>\" %s\", comment from %s.\npreventive actions:%s.", caut[caution], what, where, precaution );

  if (detail == brief)
    printf("[%s] -%s \n", where, what);
  

  if (caution == extreme) //for all extremely important messages, stop execution.
    {
      printf("\n");
      pause(Cpp_MODE);
      exit(-1);
    }

  if (caution == high) //think through this error and just move on with termination.
    pause(Cpp_MODE);

}


void pause(char * cppmode) //used while testing under c++ builder.
{
  if ( strcmp(cppmode, "no") == 0 )
    {
      if (DETAIL == all)
	printf("."); //skipping pause
    }  
  else
    {
      int a;
      scanf("%d",&a);
    }
}
