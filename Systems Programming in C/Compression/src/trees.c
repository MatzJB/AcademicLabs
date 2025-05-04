#include "trees.h"

Tree *newLeaf()
{
  Tree *p;
  if ((p = (Tree *)malloc(sizeof(Tree))) == NULL)
    disp("newLeaf()", "function Malloc()", "free some space on the HDD", extreme, DETAIL);
  return p;
}

Tree *graft(Tree *l, Tree *r, int letter, int nr)
{
  Tree *t;
  t = newLeaf(); // create a new node, allocate some mem
  t->left = l;
  t->right = r;
  t->ch = letter;
  t->freq = nr;
  return t;
}

void dispArrayOfTrees(Tree *forest[], int size)
{
  if (DETAIL == all)
  {
    printf("\n[");

    for (int i = 1; i <= size; i++)
    {
      if (i % size != 1 && size != 1)
        printf(",");

      printf("%d", forest[i - 1]->freq);
    }

    printf("] \n");
  }
}

int depth(Tree *tree)
{
  if ((tree->left == NULL) | (tree->right == NULL))
    return 0;
  else
    return 1 + max2(depth(tree->left), depth(tree->right));
}

int max2(int x, int y)
{
  return (x > y) ? x : y;
}

Tree *makeTree(RateTable frequenceArray)
{
  Tree *lost_woods[TABLE_SIZE];
  int sizeOfWoods = TABLE_SIZE, j = 0; // j is soon to be the new size

  for (int i = 0; i < sizeOfWoods; i++)
    if (frequenceArray[i] > 0)
      lost_woods[j++] = graft(NULL, NULL, i, frequenceArray[i]);

  sizeOfWoods = j; // new size of sizeOfWoods
  dispArrayOfTrees(lost_woods, sizeOfWoods);

  while (sizeOfWoods > 1)
  {
    if (DETAIL == all)
      printf("building >>");

    qsort(lost_woods, sizeOfWoods, sizeof(Tree *), compare_tree_rev);

    dispArrayOfTrees(lost_woods, sizeOfWoods);

    Tree *t1, *t2;

    t1 = lost_woods[sizeOfWoods - 1];
    t2 = lost_woods[sizeOfWoods - 2];

    Tree *t3;

    t3 = graft(t1, t2, 0, t1->freq + t2->freq);
    lost_woods[sizeOfWoods - 2] = t3; // replace the first of the two grafted trees
    sizeOfWoods--;

    if (DETAIL == all)
      printf("==> ");

    dispArrayOfTrees(lost_woods, sizeOfWoods);
    // insert the new tree t3 in the right place in the tree (next while loop does this)
  }

  assert(depth(lost_woods[0]) < j); // since depth counts nodes now (test !=1)

  if (DETAIL == all)
    printf("number of nodes %d and chars %d\n", depth(lost_woods[0]), j);

  return lost_woods[0];
}

// the comparisons are made considering the frequency
int compare_tree_rev(const void *t1, const void *t2)
{
  int t3 = (*(Tree **)t1)->freq;
  int t4 = (*(Tree **)t2)->freq;

  if (t3 < t4)
    return 1; // should be -1 if original sort
  if (t3 == t4)
    return 0;
  return -1;
}
