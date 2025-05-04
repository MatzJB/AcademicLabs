#include "lab1_standard.h"
//advanced version


//typedef FILE BitFile;


typedef enum {bitfile_mode_read, bitfile_mode_write} BitFileMode;

typedef struct _BitFile                 /* bit input/output buffer */
{
  BitFileMode mode;                   /* file mode */
  unsigned char buffer;               /* intermediate store for bits*/

  unsigned char counter;              /* how many bits are waiting */
  FILE *f;                            /* byte file */
} BitFile;

BitFile * bitOpen(FILE * f, BitFileMode mode);//, BitFileMode mode);
int getBit(BitFile *bf);
void putBit(unsigned char bit, BitFile *bf);
void putBits(unsigned nbits, unsigned long word, BitFile *bf);
void bitClose(BitFile *bf);

