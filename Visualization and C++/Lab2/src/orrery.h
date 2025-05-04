
#include "GUtil.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


void drawSphered(double r, double ALTsegs, double AZsegs, GLfloat color[], int sun);

void initStars();
void drawStars(int numstars, int lights_on);
void createMenu();
double dist(double x, double y, double z);
void printfp(const char* str, int printprob);
