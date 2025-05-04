#include "GUtil.h"
#include "village.h"

#define DEBUG 0 // if debug 1 then some printing of matrices will be done

#define QUIT 4
// #define FULLSCRN 3
#define VILLAGE 2
#define WINDOW 1
#define RESET 3

int axis_orientation = WINDOW;

int xrot = 0, yrot = 0, zrot = 0;
int on = 0;
int reset_look = 0;

#define MAX_SIZE 100000
#define escape break

// float PI=3.1415826;
int light = 1, obj = 1, degree = 10, rot = 0;
float model_view[16];
GLdouble local[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

void lookat();

int invmouse = -1;
int fullscreen = -1;

void resetlocal()
{
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX, local);
  lookat();
}

void menu_select(int item)
{
  if (item) // item!=0
  {
    resetlocal();
    // reset_look=1;
    glutPostRedisplay();
  }

  if (item == WINDOW)
    axis_orientation = WINDOW; // axis type used
  else if (item == VILLAGE)
    axis_orientation = VILLAGE;
  else if (item == RESET)
  {
    resetlocal();
  }

  if (item == QUIT)
    exit(0);

  createMenu();
}

void createMenu()
{
  glutCreateMenu(menu_select);

  glutAddMenuEntry("Village", VILLAGE);
  glutAddMenuEntry("Window", WINDOW);
  glutAddMenuEntry("Reset", RESET);
  glutAddMenuEntry("Quit", QUIT);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int xupdated = 0, yupdated = 0, zupdated = 0;

int angle = 5;
int step = 10;
static void key(unsigned char key, int kx, int ky)
{

  xupdated = 0;
  yupdated = 0;
  zupdated = 0;

  if (key == 'q')
    exit(1);

  if (key == 't') // toggle orientation
  {
    // resetlocal();
    if (axis_orientation == WINDOW)
      axis_orientation = VILLAGE;
    else
      axis_orientation = WINDOW;
  }

  if (axis_orientation == WINDOW)
  {
    glPushMatrix();
    {
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);

      if (key == 'x')
      {
        glRotated(-step, 1, 0, 0);
        xupdated = 1;
      }
      if (key == 'y')
      {
        glRotated(-step, 0, 1, 0);
        yupdated = 1;
      }
      if (key == 'z')
      {
        glRotated(-step, 0, 0, 1);
        zupdated = 1;
      }
      if (key == 'X')
      {
        glRotated(step, 1, 0, 0);
        xupdated = 1;
      }
      if (key == 'Y')
      {
        glRotated(step, 0, 1, 0);
        yupdated = 1;
      }
      if (key == 'Z')
      {
        glRotated(step, 0, 0, 1);
        zupdated = 1;
      }

      glMultMatrixd(local);                     // multiply with model-view
      glGetDoublev(GL_MODELVIEW_MATRIX, local); // save model view in local
    }
    glPopMatrix();
  }

  else if (axis_orientation == VILLAGE) // local rotations
  {

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glMultMatrixd(local);

    if (key == 'x')
    {
      glRotated(-step, 1, 0, 0);
      xupdated = 1;
    }
    if (key == 'y')
    {
      glRotated(-step, 0, 1, 0);
      yupdated = 1;
    }
    if (key == 'z')
    {
      glRotated(-step, 0, 0, 1);
      zupdated = 1;
    }
    if (key == 'X')
    {
      glRotated(step, 1, 0, 0);
      xupdated = 1;
    }
    if (key == 'Y')
    {
      glRotated(step, 0, 1, 0);
      yupdated = 1;
    }
    if (key == 'Z')
    {
      glRotated(step, 0, 0, 1);
      zupdated = 1;
    }

    glGetDoublev(GL_MODELVIEW_MATRIX, local); // save  modelview in local
  }

  glutPostRedisplay();
}

/*
The idea is to store the rotations in a matrix (local) that refers to the
local axes (for the buildings). For each button press, 'local' will be
updated with new rotations (accumulated from the previous ones). This is because
rotations do not commute; you can't simply add rotations without considering the order.
*/

void display(void)
{
  double red[3] = {1, 0, 0};
  double black[3] = {0, 0, 0};
  double white[3] = {1, 1, 1};
  double blue[3] = {0, 0, 1};

  GLdouble temp[16];

  glMatrixMode(GL_MODELVIEW);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  lookat();

  if (axis_orientation == WINDOW)
    drawAxis(60, 4, 1, 0, 0, 0);
  else
    drawAxis(60, 4, 0, 0, 0, 0);

  glPushMatrix();
  {
    glLoadIdentity();
    lookat();
    glMultMatrixd(local);

    glGetDoublev(GL_MODELVIEW_MATRIX, temp);

    if (DEBUG)
      printMatrix("vm:", temp, 5);

    drawCity();

    if (axis_orientation == VILLAGE)
      drawAxis(50, 4, 1, 0, 0, 0);
    else
      drawAxis(50, 4, 0, 0, 0, 0);
  }
  glPopMatrix();

  glutSwapBuffers();
}

void lookat()
{
  gluLookAt(0, 0, 200,
            0, 0, 0,
            0, 1, 0);
}

void drawSky()
{
  glColor3d(135.0 / 255.0, 200 / 255.0, 250.0 / 255.0); // light skyblue
  glutSolidCube(1000);
}

void drawCity()
{
  drawSky();
  DrawGrass(-50, 50, -50, 50);

  glPushMatrix();
  {
    drawBuildings();
  }
  glPopMatrix();
}

/*
  Random grass color
*/
void DrawGrass(int minx, int maxx, int minz, int maxz)
{

  glColor3f(0.1, 0.6, 0.0);

  glBegin(GL_QUADS);
  {
    glVertex3i(minx, 0, minz);
    glVertex3i(maxx, 0, minz);
    glVertex3i(maxx, 0, maxz);
    glVertex3i(minx, 0, maxz);
  }
  glEnd();
}


void DrawHouse() // ROWS-[0,1]*ROWS --> ROWS-[0,ROWS] --> [ROWS,0]
{
  float r, g, b;
  r = 0.7;
  g = 0;
  g = 0;

  glPushMatrix();
  {
    // Taket
    glColor3f(0.7, 0, 0);
    glTranslatef(-5, -5, 0);

    glBegin(GL_QUAD_STRIP);
    {
      glVertex3f(0, 10, 10);
      glVertex3f(0, 10, 0);

      glVertex3f(5, 15, 10);
      glVertex3f(5, 15, 0);

      glVertex3f(10, 10, 10);
      glVertex3f(10, 10, 0);

      ////glVertex3f(0, 10, 10);
      ////glVertex3f(0, 10, 0);
    }
    glEnd();

    glColor3f(0., 0., 1.);

    glColor3f(1., 0., .0);
    glBegin(GL_TRIANGLE_FAN);
    {
      glVertex3f(0, 10, 10);
      glVertex3f(5, 15, 10);

      glVertex3f(10, 10, 10);
      glVertex3f(0, 10, 10);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    {
      glVertex3f(0, 10, 0);
      glVertex3f(5, 15, 0);

      glVertex3f(10, 10, 0);
      glVertex3f(0, 10, 0);
    }
    glEnd();
  }

  glPopMatrix();

  glPushMatrix();
  {
    glTranslatef(0, 0, 5);
    glColor3f(0, 0.1, 0.4);
    glutSolidCube(10);
    glTranslatef(0, 9, 1);
    glutSolidCube(3);
  }
  glPopMatrix();
}

void drawBuildings()
{
  float r = 1.0, g = 0.0, b = 0.0;

  glPushMatrix();
  {
    glTranslatef(-20, 5.5, -20 * 2 - 5);

    int k;
    for (k = 0; k < 3; k++)
    {
      glTranslatef(0, 0, 20); //
      DrawHouse();

      glPushMatrix();
      {
        glTranslatef(20, 0, 0);
        DrawHouse();
        glTranslatef(20, 0, 0);
        DrawHouse();
      }
      glPopMatrix();
    }
  }
  glPopMatrix();
}

void myReshape(int w, int h)
{
  int border = 10;

  int size;
  int xmin;
  int ymin;

  if (w > h) // w is deciding the size of the window
  {
    if (h < 2 * border)
      border = 0; // not necessary to use border in this case

    size = h - 2 * border;
    xmin = 0.5 * (w - size);
    ymin = border;
  }
  else // we let h decide
  {
    if (w < 2 * border)
      border = 0; // not necessary to use border in this case
    size = w - 2 * border;
    xmin = border;
    ymin = 0.5 * (h - size);
  }

  glViewport(xmin, ymin, size, size);

  //////////////////////

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  gluPerspective(42, 1, 0.1, 10000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  lookat();

  glGetFloatv(GL_MODELVIEW_MATRIX, model_view);
}

void rotate()
{
  rot += 5;
  glutTimerFunc(50, rotate, 0);
  glutPostRedisplay();
}

void init()
{
  printMatrix("local init", local, 100);
  createMenu();
}

int main(int argc, char **argv)
{
  printf("Village rotations by Matz J\n");
  printf("Use t for toggle between rotaional axis (shown in red) doesn't reset the rotations.\n");

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);

  glutCreateWindow("Village");

  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(key);

  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(10, rotate, 0);
  
  if (DEBUG)
    printMatrix("local", local, 100);
  init();

  glutMainLoop();
  return 0;
}
