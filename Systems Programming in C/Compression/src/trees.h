#include "lab1_standard.h"

Tree *makeTree(RateTable freq);
Tree *graft(Tree *l, Tree *r, int letter, int nr);
Tree *newLeaf(void);
int compare_tree_rev(const void *t1, const void *t2);
void dispArrayOfTrees(Tree *forest[], int size); // for testing
// void preorder(int level, Tree * tree, int array[]);
int depth(Tree *tree);
int max2(int x, int y);

void preorder(Tree *tree);
Tree *makeTree(RateTable frequenceArray);
