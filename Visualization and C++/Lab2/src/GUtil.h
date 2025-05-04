
#ifndef _GUTILH_
#define _GUTILH_

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


//#define SWITCH_LIGHT if (lights_on){glDisable(GL_LIGHTING); glDisable(GL_LIGHT0); } else {glEnabable(GL_LIGHTING); glEnabable(GL_LIGHT0); }



#define PAUSE scanf("%d", stdin);
//#define glEnabel(s) glEnable(s) //funny
void drawAxis(int length, double strength, double r, double g, double b, int lights_on);

void printMatrix(const char*,double m[], int prob);
void printStrokedString(char * s);

void drawGrid(int density, double strength, int min, int max,int);
//sphere
//
extern int lights_on;

#endif

