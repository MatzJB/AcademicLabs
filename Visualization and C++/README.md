
# Matlab, getting started


## Eigenvalues

Arnoldi’s method is an iterative algorithm used to compute a few eigenvalues and corresponding eigenvectors of a large, sparse, and generally non-Hermitian matrix $A$. At each iteration $k$, the method generates $k$ approximate eigenvalues, though typically only a few are accurate. For instance, with a matrix of size $n = 5000$, using $k = 50$ might yield around 5 good approximations. An important by-product of the algorithm is the **residual norm**, which measures the quality of the computed eigenpairs.



$$
\frac{\| A x - \lambda x \|}{\| x \|}
$$

For an approximate eigenpair $(\lambda, x)$, where $\lambda$ is the approximate eigenvalue and $x$ is the corresponding eigenvector, the **residual norm** can be used to identify good approximations. The iteration continues until the desired number of accurate eigenpairs has been obtained.


## Stability

In the study of the stability of ODE solvers, it is useful to visualize the region of absolute stability of a method. This region is a set in the complex plane where the modulus of a characteristic function is less than one.

For two well-known Runge-Kutta methods, the characteristic functions are:

- **RKF45**: $p(h) = 1 + h + \frac{h^2}{2} + \frac{h^3}{6} + \frac{h^4}{24} + \frac{h^5}{104}$
- **DOPRI(5, 4)**: $p(h) = 1 + h + \frac{h^2}{2} + \frac{h^3}{6} + \frac{h^4}{24} + \frac{h^5}{120} + \frac{h^6}{600}$

The region of interest is where $|p(h)| < 1$ in the complex plane.


## Perception

Draw a sequence of 40 adjacent vertical rectangles in a window. Colour the rectangles using a gray scale;  the leftmost rectangle should be white and the rightmost black. Set the Edgecolor-property of the rectangles to None. Look at the border between rectangles. Do you notice anything special?  The visual system is sensitive to changes, it does not measure absolute values.

## Two 3D-problems

### Problem 1
A square matrix A of order n has n² elements, and we can think of it as a point in an n²-dimensional space. The set of all singular matrices of order n forms a surface in this space, and any specific singular matrix is a point on that surface.

For n = 2, where the matrix is real and symmetric, there are only three free parameters. This makes it possible to visualize the surface near the origin (covering all eight octants). We can also mark the matrices [1 0; 0 1] and [1 0; 0 0.01] and the corresponding closest (in || ||₂) singular matrices, which are [1 0; 0 0] in both cases (though not unique in the first). 

This visualization helps explain the condition number 

$$
k(A) = ||A|| ||A^{-1}||
$$

for the $$Ax = b$$ problem. A high condition number (large $$k(A)$$) indicates the matrix is close to the surface, suggesting an ill-conditioned problem. A low condition number (small $$k(A)$$) indicates the matrix is far from the surface, signifying a well-conditioned problem.

### Problem 2
To visualize the set of points where the polynomial \(x^3 + ax^2 + bx + c\) has multiple zeros (of multiplicity two or three), you can plot the discriminant of the cubic equation. The discriminant is zero at the points where the polynomial has multiple roots, and it is also zero at the points with three identical roots. 

- The set of \((a, b, c)\) for which all roots are real corresponds to regions where the discriminant is non-negative. 
- If the discriminant is negative, at least one root has a non-zero imaginary part.

The zeros of a polynomial are continuous functions of the coefficients.


## Scattering
![scatter2](https://github.com/user-attachments/assets/37bdb54f-b0da-4f48-83ae-269d254f9507)

A common visualization problem is to draw curves in three dimensions. We may want to draw streamlines for a fluid or gas, for example. It may be difficult to interpret such plots, since it is hard to see if one curve lies behind another. So, it is common to draw the curves as ribbon or tubes instead. In the images below the leftmost curve is drawn using the plot3-command and in the other two the curve has been drawn as a tube. The leftmost curve is almost impossible to interpret (at least if one has not seen the tubes).

The gray spheres are the stationary charged particles (they should be points, but then we cannot see them clearly).

Now some mathematics. Let `r(t) = (x(t), y(t), z(t))` be the position of one moving particle. Assuming Coulomb forces (no gravity, etc.), the differential equation for the motion is given by (dropping `(t)` in `r(t)`):

$$
r'' = c * \left[ \frac{r - p_1}{\|r - p_1\|^3} + \frac{r - p_2}{\|r - p_2\|^3} + \dots + \frac{r - p_n}{\|r - p_n\|^3} \right]
$$



where `p1, ..., pn` are the positions of the `n` fixed particles. `c` is a constant having to do with the mass of the particle and the force between particles. `|| ||` denotes the ordinary two-norm (distance). Use `ode45` to solve the system. Test at least the following configuration, defined by the code below:

```matlab
global P c  % P and c are used in f

% P contains the coordinates for the fixed particles. One particle per column.
P = [
 -0.0466  0.0368  0.1156 -0.1553 -0.1079  0.1564  0.1155  0.1430  0.0826 0.1333
 -0.0516 -0.0825 -0.0364 -0.1542  0.0528 -0.1747  0.0316  0.0713  0.1680 0.0453
  0.1293  0.0281 -0.1349 -0.1852  0.1727 -0.0044 -0.1318 -0.1272 -0.1713 0.0056
];

% The constant c is 0.001.
c = 0.001;

tspan = linspace(0, 15, 50); 
phi_s = linspace(0, 2 * pi, 8);

% Find the trajectories for a few particles...
for R = [0.1 0.2 0.3]  % R is the radius, and not r
  for phi = phi_s(1:end-1)
    [t, y] = ode45(@f, tspan, [2 R*cos(phi) R*sin(phi) -0.3 0 0]');
    % more stuff ...
  end
end
```



## Shadows and animation
![shadows](https://github.com/user-attachments/assets/fc78a37c-cf73-4d3f-a6e6-adcd85b8e19f)

In this assignment, you will build a MATLAB GUI to rotate a 3D convex body, animate the motion, and simulate shadows. The body can be an ellipsoid, cylinder, or box, and is defined using built-in surface-generating functions. Rotation occurs around the center of mass or another chosen internal point.

Use the GUI to select the axis of rotation (x, y, or z) and to start/stop the animation, quit, or capture frames into an AVI file. The GUI must include:

Buttons: x, y, z, Start, Stop, Quit, Capture

A menu named Figure to choose body type (ellipsoid, cylinder, box)

Display of current rotation axis and body state

Shadow projections on the coordinate planes using orthogonal projection (simulating a distant light source)

Use convhull to ensure the surface is convex and avifile for capturing animations. Optimize animation by updating graphics object data directly and setting the figure 'Renderer' to 'opengl'.

## Moving curves


This simplified lab is inspired by a real application for a physics research project where multiple data curves needed visual comparison. The core feature is a MATLAB GUI allowing users to interactively move plotted curves for visual alignment.

Features
Log-log plot of multiple curves (data sets).

Click-to-select: Click once on a curve to attach it to the cursor. The cursor changes to a hand icon.

Drag-to-move: Move the selected curve freely with the mouse.

Click-to-release: A second click drops the curve at the new location; the cursor returns to default.

Axes remain fixed: The plot window does not rescale during interaction; curves may partially leave the visible area.

Curve shapes remain unchanged: Only their positions are affected—no distortion or resampling.

Usage
This tool enables quick visual comparison of experimental data (e.g. peak alignment), particularly helpful when overlaying similar measurements. It’s lightweight, user-friendly, and designed for scientific analysis.

![pb2](https://github.com/user-attachments/assets/8a78e286-6a21-4756-809c-267f2ab36936)
 
# OpenGL

## Orrery

information to be added soon

## Village

information to be added soon
 
# IBM OpenDX
![OpenDX](https://github.com/user-attachments/assets/47ae92bc-bcba-48cc-9c0f-689c2c8fb5be)

## Streamlines

information to be added soon

## Igloo

information to be added soon
