

/***************************
 *                         *
 *   Compression program   *
 *   by Matz Johansson B   *
 *                         *
 ***************************/

#include "lab1_standard.h"
#include "frequence.h"
#include "bitsequences.h"
#include "huffman.h"
#include "trees.h"

void putTree(Tree *tree, BitFile *bf);
Tree *getTree(BitFile *file);
void preorder(Tree *tree);
void encode(FILE *text, Tree *tree, BitFile *bf);
void decode(FILE *text, Tree *tree, BitFile *bf, unsigned long filesize);
void decompress(BitFile *bf, FILE *text);
void compress(FILE *text, BitFile *bf);

void putTree(Tree *tree, BitFile *bf)
{
  if (tree->left == NULL)
  {
    putBit(0, bf); // beginning of a char
    putBits(8, tree->ch, bf);
  }
  else // a node
  {
    putBit(1, bf);
    if (tree->left != NULL)
      putTree(tree->left, bf);
    if (tree->right != NULL)
      putTree(tree->right, bf);
  }
}

Tree *getTree(BitFile *file)
{
  int theBit;
  theBit = getBit(file);
  Tree *tree;

  if (theBit == 0) // we reached a char (the end before NULLs in a tree)
  {
    char byte = 0;

    for (int i = 1; i <= 8; i++)         // fetch one byte
      byte = (byte << 1) | getBit(file); // or + instead of |

    if (DETAIL == all)
      printf(".");

    tree = graft(NULL, NULL, byte, 0); // freq does not matter here
  }
  else if (theBit == 1) // we know it is a node
  {

    Tree *left = getTree(file);
    Tree *right = getTree(file);
    tree = graft(left, right, 0, 0);
  }

  return tree;
}

void preorder(Tree *tree)
{
  if (DETAIL == all)
  {
    if (tree->left == NULL)
      printf("%c|", tree->ch);
    else
    {
      printf("<");
      preorder(tree->left);
      printf(">");
      preorder(tree->right);
    }
  }
}

void encode(FILE *text, Tree *tree, BitFile *bf)
{
  HuffmanTable table;
  resetHuffman(table, TABLE_SIZE);
  tableBuilder(0, 0, tree, table);
  dispHuffmanTable(table, TABLE_SIZE);

  int letter = getc(text);

  while (letter != EOF)
  {
    putBits(table[letter].bits, table[letter].bitmask, bf);
    letter = getc(text);
  }
}

void decode(FILE *text, Tree *tree, BitFile *bf, unsigned long filesize)
{
  Tree *ant;
  unsigned char bit;

  while (filesize > 0)
  {
    ant = tree;
    bit = 0;

    while (ant->left != NULL)
    {
      bit = getBit(bf);

      if (bit == 0)
        ant = ant->left;

      if (bit == 1)
        ant = ant->right;
    }

    putc(ant->ch, text);
    filesize--;
  }
}

void compress(FILE *text, BitFile *bf)
{
  RateTable dist;
  unsigned long filesize = 0;

  collectRate(text, dist, &filesize);

  if (DETAIL == all)
    printf("\ncalculated filesize:%lu\n", filesize);

  if (filesize != 0)
  {
    Tree *tree = makeTree(dist);

    if (DETAIL == all)
      printf("traversal over original tree=");
    preorder(tree);

    rewind(text);              // go back to the beginning of the textfile
    putBits(32, filesize, bf); // put the size of the file 'text'
    putTree(tree, bf);
    encode(text, tree, bf);
  }

  // special case handling, do nothing (best compression)
}

void decompress(BitFile *bf, FILE *text)
{
  unsigned long bit = getBit(bf);

  if (bit != 2) // if bits==2 file is empty
  {
    for (int i = 1; i < 32; i++)
      bit = (bit << 1) | getBit(bf);

    Tree *tree;

    unsigned long filesize = bit;

    if (filesize != 0)
    {
      tree = getTree(bf);

      if (DETAIL == all)
        printf("the tree now looks like:  ");
      preorder(tree);

      decode(text, tree, bf, filesize);
    }
  }
}

int main(int argc, char *argv[])
{
  // Compression
  if (DETAIL == all)
    printf("^ under binary numbers means -written bits-");

  if (argc == 3)
  {
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (in == NULL)
      disp("main()", "could not find first indata", "make sure you type the right filename", extreme, DETAIL);

    BitFile *bf_out = bitOpen(out, bitfile_mode_write);

    if (DETAIL == all)
      printf("\ncompression of file:%s to a file:%s \n ", argv[1], argv[2]);

    compress(in, bf_out);
    bitClose(bf_out);
    pause(Cpp_MODE);
  }
  // Decompression
  else if (argc == 4 && (0 == strcmp(argv[1], "-d")))
  {
    FILE *in2 = fopen(argv[2], "r");
    if (in2 == NULL)
      disp("main()", "Decompression aborted", "make sure filename exist", extreme, DETAIL);

    FILE *out = fopen(argv[3], "w");

    BitFile *bf_in = bitOpen(in2, bitfile_mode_read);
    decompress(bf_in, out);

    bitClose(bf_in);
    fclose(out);
    pause(Cpp_MODE);
  }
  else
  {
    disp("main()", "wrong number of arguments", "type in 3 or 4 arguments, see documentation", extreme, DETAIL);
    pause(Cpp_MODE);
  }

  return 0;
}
