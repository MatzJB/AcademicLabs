#include "orrery.h"

/*
  TODO: split up in smaller files
*/

#define QUIT 3
#define FULLIGHT 1
#define DARKNESS 0
#define STARS_MAX 80000

double speed = 0.001; // the speed of the rotations
float PI = 3.14159265358;


int light = 1, obj = 1, degree = 10, rot = 0;
int lights_on;

GLdouble model_view[16];
GLdouble env_matrix[16];

// MOUSE HANDLING---

int invmouse = -1;
int fullscreen = -1;

typedef struct
{
  GLdouble x, y, z;
  GLdouble r, g, b;
  GLfloat size;
} star;

star stars[STARS_MAX]; // list of stars used as backdrop, it is following camera translations

void menu_select(int item)
{

  if (item == DARKNESS)
  {
    lights_on = 1;
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    printf("\tdarkness (i.e. sun emits light)\n");
  }
  else if (item == FULLIGHT)
  {
    lights_on = 0;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    printf("\tFull light (no lighting)\n");
  }

  if (item == QUIT)
    exit(0);

  createMenu();
}

void createMenu()
{
  glutCreateMenu(menu_select);

  glutAddMenuEntry("Full light", FULLIGHT);
  glutAddMenuEntry("Darkness", DARKNESS);
  glutAddMenuEntry("Quit", QUIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int lx = -50, ly = -50;
int dx, dy;

void movemouse(int mx, int my)
{
  dx = mx - lx;
  dy = my - ly;
}

int step = 4;
int on = 0;
static void specialKey(int key, int kx, int ky)
{
  glLoadIdentity();

  if (key == GLUT_KEY_UP)
  {
    glRotated(step, 1, 0, 0);
  }
  else if (key == GLUT_KEY_DOWN)
  {
    glRotated(-step, 1, 0, 0);
  }
  else if (key == GLUT_KEY_LEFT)
  {
    glRotated(-step, 0, 1, 0);
  }
  else if (key == GLUT_KEY_RIGHT)
  {
    glRotated(step, 0, 1, 0);
  }

  glMultMatrixd(model_view);
  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
  glutPostRedisplay();
}

// KEYBOARD HANDLING --
char str[1024];
int angle = 1;
static void key(unsigned char key, int kx, int ky)
{
  glLoadIdentity();

  if (key == 'w') // walk forward
  {
    glTranslated(0, 0, step);
  }
  else if (key == 's') // walk backwards
  {
    glTranslated(0, 0, -step);
  }
  else if (key == 'a') // strafe left
  {
    glTranslated(step, 0, 0);
  }
  else if (key == 'd') // strafe right
  {
    glTranslated(-step, 0, 0);
  }
  else if (key == 'r') // up
  {
    glTranslated(0, -step, 0);
  }
  else if (key == 'f') // down
  {
    glTranslated(0, step, 0);
  }

  if (key == '+')
  {
    speed += 7E-4;
  }
  if (key == '-')
  {
    speed -= 7E-4;
  }

  if (key == '*')
  {
    speed *= 1.04;
  }

  if (key == '/')
  {
    speed /= 1.04;
  }

  if (key == 't') // toggle lights
  {
    if (lights_on)
    {
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
    }
    else
    {
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
    }

    lights_on = -lights_on + 1; // toggle
  }

  if (key == 27 || key == 'q')
    exit(0);

  glMultMatrixd(model_view);
  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

  glutPostRedisplay();
}

GLfloat sun_diffuse[] = {1., 1., 0.}; // used as light color
GLfloat sun_surface_color[] = {1., .8, 0.};
GLfloat sun_position[] = {0., 0., 0., 1.};

GLfloat earthcolor[] = {.1, .0, 1.};
GLfloat mooncolor[] = {.6, .6, .7};

double earthrot = 0;
double earthaxisrot = 0;
double moonrot = 0;

void idle()
{
  earthrot += speed * 360 / 365.;
  earthaxisrot += speed * 360.;
  moonrot += speed * 360 / 30.;
  // earthrot += speed*365;
  // earthaxisrot += speed;
  // moonrot += speed*30;

  glutPostRedisplay();
}

// GLfloat sun_diffuse[3];
int time = 0;

void display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLdouble notrans_matrix[16];

  int i;

  for (i = 0; i < 16; i++)
    notrans_matrix[i] = model_view[i];

  notrans_matrix[12] = 0;
  notrans_matrix[13] = 0;
  notrans_matrix[14] = 0;

  int sphereres = 60;

  glPushMatrix();
  {
    glLoadIdentity();
    glMultMatrixd(notrans_matrix);
    drawStars(6000, lights_on);
  }
  glPopMatrix();

  drawGrid(8, 1, -100, 100, lights_on);

  glPushMatrix();
  {
    glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
    drawAxis(90, 2, 1, 0, 0, lights_on);
    drawSphered(30, sphereres, sphereres, sun_surface_color, 1);
  }
  glPopMatrix();

  glPushMatrix();
  {
    // drawGrid(10, -300, 300);
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, suncolor);
    //    drawSphered(20, sphereres, sphereres, suncolor);
    // glutSolidSphere(1., 20., 16.);

    ////drawAxis(50, 2, 1, 0, 1, lights_on);

    glRotated(earthrot, 0, 1, 0);
    glTranslated(100, 0, 0);
    glRotated(-earthrot, 0, 1, 0);

    glPushMatrix();
    {
      glRotated(23.4, 1, 0, 0);
      glRotated(earthaxisrot, 0, 1, 0);
      drawSphered(10, sphereres, sphereres, earthcolor, 0);
      drawAxis(40, 2, 0.4, 0.4, 1, lights_on);
    }
    glPopMatrix();

    // Moon rotates
    glRotated(moonrot, 0, 1, 0);
    glTranslated(30, 0, 0); //translate moon and rotate wrt origin (i.e. earth)

    // glMaterialfv(GL_FRONT, GL_DIFFUSE, mooncolor);
    drawSphered(3, sphereres, sphereres, mooncolor, 0);
    drawAxis(40, 3, 0.6, 0.6, 0.8, lights_on);
  }
  glPopMatrix();

  //  exit(1);

  glutSwapBuffers();
}

// takes the delta (x,y,z)
double dist(double dx, double dy, double dz)
{
  return sqrt(dx * dx + dy * dy + dz * dz);
}

GLfloat konc = 5;

void drawSphered(double r, double ALTsegs, double AZsegs, GLfloat color[], int sun)
{

  double ALTdelta = 2 * PI / ALTsegs;
  double AZdelta = PI / AZsegs; // range is pi [abs(-pi/2)+abs(pi/2)];
  double r2 = r * 1.2, phi, theta, d;

  double X[3], Y[3], Z[3];
  int i;
  int the_switch = 0;

  // glMaterialfv(GL_BACK_AND_FRONT, GL_DIFFUSE, color);
  if (sun)
  {
    if (lights_on)
    {
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      the_switch = 1;
      glColor3d(color[0], color[1], color[2]);
    }
  }

  if (lights_on)
  {
    GLfloat specular[] = {0.6, 0.6, 0.6};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

    glMaterialfv(GL_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color); // ambient och diffuse are same
    glMaterialfv(GL_BACK, GL_AMBIENT, color);
  }
  else if (~lights_on)
  {
    glColor3d(color[0], color[1], color[2]);
  }

  for (phi = 0; phi < PI; phi += AZdelta)
    for (theta = 0; theta < 2 * PI; theta += ALTdelta)
    {
      X[0] = sin(theta) * sin(phi);
      X[1] = sin(theta + ALTdelta) * sin(phi);
      X[2] = sin(theta + ALTdelta) * sin(phi + AZdelta);
      X[3] = sin(theta) * sin(phi + AZdelta);

      Y[0] = cos(theta) * sin(phi);
      Y[1] = cos(theta + ALTdelta) * sin(phi);
      Y[2] = cos(theta + ALTdelta) * sin(phi + AZdelta);
      Y[3] = cos(theta) * sin(phi + AZdelta);

      Z[0] = cos(phi);
      Z[1] = cos(phi);
      Z[2] = cos(phi + AZdelta);
      Z[3] = cos(phi + AZdelta);

      glBegin(GL_QUADS);
      {
        for (i = 0; i <= 3; i++)
        {

          d = dist(r2 * X[i] - r * X[i],
                   r2 * Y[i] - r * Y[i],
                   r2 * Z[i] - r * Z[i]);

          glNormal3d((r2 * X[i] - r * X[i]) / d,
                     (r2 * Y[i] - r * Y[i]) / d,
                     (r2 * Z[i] - r * Z[i]) / d);

          glVertex3d(r * X[i], r * Y[i], r * Z[i]);
        }

        /*
          glVertex3d(r*X[0], r*Y[0], r*Z[0]);
          glVertex3d(r*X[1], r*Y[1], r*Z[1]);
          glVertex3d(r*X[2], r*Y[2], r*Z[2]);
          glVertex3d(r*X[3], r*Y[3], r*Z[3]);
        */
      }
      glEnd();

    } // for

  if (sun)
    if (the_switch) // we know we switched the lights off
    {
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
    }
}

void initStars(int nstars)
{

  GLdouble r = 1000, phi, theta;
  // int starprob = 5;
  GLdouble x, y, z;
  int i, c = 0;

  GLdouble size[2]; // min max
  glGetDoublev(GL_POINT_SIZE_RANGE, size);
  GLdouble gran[2]; // min max
  glGetDoublev(GL_POINT_SIZE_GRANULARITY, gran);

  printf("POINT_SIZE:\n");
  printf("range <- [%.2f,%.2f]\tgranularity:%.2f\n", size[0], size[1], gran[0]);

  if (nstars > STARS_MAX) // overriding
    nstars = STARS_MAX;

  while (c < nstars)
  {
    if (c >= nstars) // quit
      break;
    {
      theta = ((rand() % 1000) / 999.) * 2 * PI; // couple of decimals [0, 2*PI]
      phi = ((rand() % 1000) / 999.) * PI;

      x = sin(theta) * sin(phi);
      y = cos(theta) * sin(phi);
      z = cos(phi);

      // the color of a star is random, no color channel is favored
      stars[c].r = rand() % 1000 / 999.; // 1-( ( rand()%100 )/99. )/2+1/2;//ljusare f�rg <-[0, 1/2]
      stars[c].g = 1 - ((rand() % 100) / 99.) / 2 + 1 / 2;
      stars[c].b = 1 - ((rand() % 100) / 99.) / 2 + 1 / 2;

      stars[c].x = r * x;
      stars[c].y = r * y;
      stars[c].z = r * z;

      stars[c].size = trunc(3 * (rand() % 100) / 99.) + 1; // ger storlek av stj�rna, [1,4]
      printf("#%l.4d size:%f, \tpos:(%.3f %.3f %.3f)%\n", c, stars[c].size, stars[c].x, stars[c].y, stars[c].z);
      c++; // next star, please!
    }

  } // while

  printf("created %d stars\n", c);
  // exit(1);//ta bort
}

/*
The stars are made from the same algorithm as the sphere, but instead
of quads I use points, and they are randomized and put in an array of
structs consisting of color information and size (and distance?) ...  The
starfield are moving with the camera, so the translations are not
affecting the starfield...
*/

void drawStars(int numstars, int lights_on)
{
  int c;

  if (numstars > STARS_MAX)
    numstars = STARS_MAX;


  if (lights_on)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }

  for (c = 0; c < numstars; c++)
  {
    glPointSize(stars[c].size);
    glColor3d(stars[c].r, stars[c].g, stars[c].b);

    glBegin(GL_POINTS);
    {
      glVertex3d(stars[c].x, stars[c].y, stars[c].z);
    }
    glEnd();
  }


  if (lights_on)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }
}

void drawSky()
{
  glColor3d(135.0 / 255.0, 206.0 / 255.0, 250.0 / 255.0); // light skyblue
  glutSolidCube(1000);
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

  glMatrixMode(GL_PROJECTION);


  glLoadIdentity();
  gluPerspective(42, 1, 0.1, 10000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(200, 100, 200,
            0, 0, 0,
            0, 1, 0);

  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
}

void rotate()
{
  // rot += 5;
  // glutTimerFunc(50,rotate,0);
  glutPostRedisplay();
}

void init()
{
  // Materialegenskaper

  GLfloat mat_specular[] = {0.9, 0.6, 0.6, 1.0};
  GLfloat mat_diffuse[] = {1, 1, 1, 1.0};
  // GLfloat mat_ambient[] = mat_diffuse;

  GLfloat mat_diffuse_back[] = {0.0, 0.0, 1.0, 1.0};

  GLfloat sun_specular[] = {1., 1., 1., 1.};
  GLfloat sun_ambient[] = {0.2, 0.2, 0.1, 1.};

  glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse); // se above display
  glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);

  /*
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_back);
  */

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  lights_on = 1;

  glEnable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);
  // glEnable(GL_NORMALIZE);
  // glEnable(GL_AUTO_NORMAL);
  glEnable(GL_POINT_SMOOTH);

  initStars(2000);
  createMenu();
}


int main(int argc, char **argv)
{
  // glutInit(&argc, argv);
  printf("Orrery by Matz Johansson\n\n");

  printf("Use arrow tangents and A,S,D and W to navigate in the orrery\n");
  printf("\tkeys +,- changes the speed of the orrery\n");

  printf("\t * / accelerates and dampens the speed, use at own risk, it may(will)\n");
  printf("\ttruncate animation resolution if overused at one session\n\n");
  printf("\t t - toggles lighting\n\n");

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Orrery");

  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(key);
  glutSpecialFunc(specialKey);
  glutPassiveMotionFunc(movemouse);

  glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
  glutIdleFunc(idle);
  glEnable(GL_DEPTH_TEST);
  //  glutTimerFunc(10, rotate,0); //animering

  init();

  glutMainLoop();
  return 0;
}
