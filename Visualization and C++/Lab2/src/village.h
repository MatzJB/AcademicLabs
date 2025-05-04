#include "GUtil.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void DrawGrass(int, int, int, int);
void drawBuildings();
void drawRoads();
void drawCity();
void createMenu();

void menu_select(int item);

void createMenu();
void movemouse(int mx, int my);
static void specialKey(int key, int kx, int ky);
static void key(unsigned char key, int kx, int ky);
void drawSky();
void drawCity();

void DrawHouse();

