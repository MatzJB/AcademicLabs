#include "lab1_standard.h"

typedef struct             //Huff
{
  unsigned int bits;      //- The number of bits
  unsigned int bitmask;   //- The bit mask, long??
} HuffmanEncoding;

typedef HuffmanEncoding HuffmanTable[TABLE_SIZE];
void tableBuilder(int level, unsigned long int bitmask, Tree * tree, HuffmanTable table);
void tableEmulator(HuffmanTable table);//for testing purposes
HuffmanEncoding bitbuilder(int bit, int bitmask);

void dispHuffmanTable(HuffmanTable table, int size);
void resetHuffman(HuffmanTable table, int size);
void tableBuilder(int level, unsigned long int bitmask, Tree * tree, HuffmanTable table);
