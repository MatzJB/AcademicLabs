
#include "huffman.h"

void tableBuilder(int level, unsigned long int bitmask, Tree * tree, HuffmanTable table)
{
      if (tree->left == NULL)//base case
	{
	  table[tree->ch].bits    = level;
	  table[tree->ch].bitmask = bitmask;
	}
      else
	{
	  tableBuilder(level+1, (bitmask<<1)|1, tree->right, table);
	  tableBuilder(level+1, (bitmask<<1), tree->left, table);
	}
}


void dispHuffmanTable(HuffmanTable table, int size)
{
  if (DETAIL == all)
    {

      printf(".:");

      for (int i=0; i<size; i++)
	{
	  if (table[i].bits!=0)
	    {
	      printf("{");
	      bit_print(table[i].bitmask, table[i].bits);
	      printf("}");
	    }
	}

      printf(":. \n");
    }
}


void resetHuffman(HuffmanTable table, int size)//reset the huffman encodings
{
  for (int i=0; i<size; i++)
    {
      table[i].bitmask = 0;
      table[i].bits    = 0;
    }
}

