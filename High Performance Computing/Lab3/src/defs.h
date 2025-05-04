typedef struct complex {
  double          r, i;
} Complex;

Complex Add(Complex z, Complex w);
Complex Sub(Complex z, Complex w);
Complex Mul(Complex z, Complex w);
Complex Div(Complex z, Complex w);
Complex Adds(double a, Complex z);
Complex Muls(double a, Complex z);
Complex Divs(double a, Complex z);
Complex Double2Complex(double re, double im);
Complex Exp(Complex z);
double  Cabs(Complex z, Complex w);
void    PrintC(Complex z);

void OpenWindow(int width, int height);
void CloseWindow();
void FlushWindow();
void DrawLine(int y, int line[]);
void DrawPoint(int x, int y, int color);
void DrawCircle(int x, int y, int radius, int color);
void WaitForButton(int *x, int *y, int *button);
void DrawRectangle(int x, int y, int width, int height);
void FillRectangle(int x, int y, int width, int height);
void SetColor(int);
void FillArc(int x, int y, int width, int height, int angle1, int angle2);
