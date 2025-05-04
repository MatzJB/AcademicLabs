#ifndef _LAB1_STANDARDH_
#define _LAB1_STANDARDH_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#define TABLE_SIZE 256

#define DETAIL brief
#define Cpp_MODE "no" //defines pause to work like c or c++

typedef enum {nothing,brief,verbose,all} MessageDetail;//the detail of the dispmessages
typedef enum {extreme,high,important,serious,warning,observe,did_you_know} CautionLevel;

void disp(char * where, char * what, char * precaution, CautionLevel caution, MessageDetail detail);
void pause(char * mode);

typedef unsigned long RateTable[TABLE_SIZE];

typedef struct _Tree
{
  struct _Tree * left;    // lef t Node (without info)
  struct _Tree * right;   // right Node
  int ch;                 // (character) only in a leaf
  int freq;               // occurence rate of the character
} Tree;

void bit_print(int a, int nrofbits);

#endif
