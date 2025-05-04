/*
Matz Johansson B.
Utilities for second lab in 'Scientific visualization'
used by village.c and orrery.c
*/

#include "GUtil.h"

// float PI=3.1415826;

void printfp(const char *str, int printprob)
{
  if (ceil(rand() % 100) < printprob)
    printf("%s\n", str);
}

void printMatrix(const char *str, GLdouble m[], int printprob)
{
  int i;

  if (ceil(rand() % 100) < printprob)
  {
    printf("%s\n", str);
    for (i = 0; i < 4; i++)
      printf("%.3f %.3f %.3f %.3f\n", m[i], m[4 + i], m[8 + i], m[12 + i]);
    printf("\n");
  }
}

void printStrokedString(char *s)
{
  glPushMatrix();
  {
    int n = 0;

    while (n < strlen(s))
    {
      glTranslatef(0, 1, 0); // move one unit for next symbol
      glutStrokeCharacter(GLUT_STROKE_ROMAN, s[n++]);
    }
  }
  glPopMatrix();
}

void drawAxis(int length, double strength, double r, double g, double b, int lights_on)
{
  if (lights_on)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }

  glColor3f(r, g, b);

  glLineWidth(strength);

  glBegin(GL_LINES);
  {

    // X-axel
    glVertex2i(0, 0);
    glVertex2i(length, 0);

    // Y-axel
    glVertex3i(0, 0, 0);
    glVertex3i(0, length, 0);

    // Z-axel
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, length);

    // glColor3f(1.0, 1.0, 1.0);
    // glRasterPos3d(400.0, 0.0, 0.0);
  }
  glEnd();

  // Print "x"
  glPushMatrix();
  {
    glTranslatef(length, 0, 0);
    glScalef(0.1, 0.1, 0.1);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'X');
  }
  glPopMatrix();

  // Print "y"
  glPushMatrix();
  {
    glTranslatef(0, length, 0);
    glScalef(0.1, 0.1, 0.1);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'Y');
  }
  glPopMatrix();

  // Print "z"
  glPushMatrix();
  {
    glTranslatef(0, 0, length);
    glScalef(0.1, 0.1, 0.1);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'Z');
  }
  glPopMatrix();

  //  glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);

  if (lights_on) // put the lights back on
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }
}

// quad grid
void drawGrid(int density, double strength, int min, int max, int lights_on)
{
  // density, how many lines
  if (lights_on)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }

  glLineWidth(strength);
  glColor3f(0.1, 0.3, 0.1);
  int i;

  if (density % 2 != 0)
  {
    printf("ERROR [drawGrid] Density %2!=0\n");
    PAUSE;
    exit(1);
  }

  for (i = min; i <= max; i += density)
  {
    glBegin(GL_LINES);
    {
      glVertex3i(min, 0, i);
      glVertex3i(max, 0, i);

      glVertex3i(i, 0, min);
      glVertex3i(i, 0, max);
    }
    glEnd();

  } // for

  glBegin(GL_LINES);
  {
    glVertex3i(min, 0, min);
    glVertex3i(max, 0, min);
    glVertex3i(min, 0, max);
    glVertex3i(max, 0, max);
  }
  glEnd();

  if (lights_on) // put the lights back on
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }

  // glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
}
