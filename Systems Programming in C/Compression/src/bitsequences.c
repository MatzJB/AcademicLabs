#include "bitsequences.h"

BitFile * bitOpen(FILE * f, BitFileMode mode)
{
    BitFile * bf;

  if ( (bf = (BitFile *) malloc (sizeof(BitFile))) == NULL)
    disp("bitOpen()", "Error in Malloc()", "make sure you have enough space on HDD", extreme, DETAIL);

  if (f == NULL && mode == bitfile_mode_read)
    disp("bitOpen()", "File does not exist", "check you typed the right filename", extreme, DETAIL);

  if (mode == bitfile_mode_read)
    { 
      bf->mode    = mode;  //write or read
      bf->buffer  = 0;     //fill cache with first 1 byte (8 bits)
      bf->counter = 0;     //nr of bits in buffer right now
      bf->f       = f;     //give the rest of the file
   }

  else if (mode == bitfile_mode_write)
    {
      bf->mode    = mode;
      bf->buffer  = 0;
      bf->counter = 0;
      bf->f       = f;
    }
  else
    disp("bitOpen", "nonexisting mode", "make sure the mode is defined in lab1_standard.h", extreme, DETAIL);

  return bf;
}


int getBit(BitFile * bf)
{
  int ch;
  
  if (bf->mode==bitfile_mode_write)
    disp("getBit()", "mode is wrong", "make sure mode is set to 'read'", extreme, DETAIL);

  if (bf->counter == 0)
    {
      ch = fgetc(bf->f);
      
      if ( ch == EOF )
	{
	  disp("getBit()", "getBit reached EOF, either the file is empty or you must report this to Matz JB", "", observe, DETAIL);
	  return 2;//error!!	
}
      
      bf->buffer  = ch;
      bf->counter = 8;
      if (DETAIL == all) printf("\n*flush*");
    }
  
  bf->counter--;
  
  if (DETAIL == all)
    {
      printf("\n*flush*");
      printf(" %d",(bf->buffer >> (bf->counter)) & 1);
    }
  
  return (bf->buffer >> (bf->counter))  & 1;
}


void putBit(unsigned char bit, BitFile * bf)
{ 
  if (bf->mode == bitfile_mode_read)
    disp("putBit()", "mode is wrong", "make sure mode is set to 'write'", extreme, DETAIL);

  if (bf->counter > 7)
    {

      fputc(bf->buffer, bf->f);

      if (DETAIL == all)
	{
	  printf("\tflushing old buffer:");
	  bit_print(bf->buffer, 8);
	  printf("\n");
	}

      bf->buffer  = 0;
      bf->counter = 0;
    }
  

  unsigned char buffer = bf->buffer;
  
  bf->counter++;
  bf->buffer = buffer + (bit << (8-bf->counter) );
}


void putBits(unsigned nbits, unsigned long word, BitFile * bf)
{
  while(nbits > 0)
    {
      putBit(word >> (nbits - 1) & 1, bf);
      nbits--;
    }
}


void bitClose(BitFile * bf)
{
  if (bf->counter>8)//this should never happen!
    printf("\nERROR:fetched an internal error in counter used to store bits");

  if (DETAIL == all)
    {
   printf("\nFound some junk left in buffer, I am flushing it now!");
   printf("\n");
   bit_print(bf->buffer,bf->counter);
   printf("\n");

   for(int q=1; q<=bf->counter; q++)
   printf("^");

    }

   //skal vara >0
   if ( bf->counter>0 && (bf->mode == bitfile_mode_write) )
      fputc(bf->buffer, bf->f); //flush remaining bits into stream

  fclose(bf->f);
}
