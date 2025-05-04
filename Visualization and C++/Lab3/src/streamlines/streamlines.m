temp = linspace(0, 5, 20);
[t, u] = meshgrid(temp);
deltat = 0.01;
quiver(t, u, 0*t + deltat, cos(t.*u)*deltat, 'linewidth', 1);

hold off
n_lines=10;
h=streamline(t, u, 0*t + deltat, cos(t.*u)*deltat, zeros(n_lines, 1), [0:n_lines-1])

set(h,'color',[1,0,0]);
set(h,'linewidth',3);

hold on

axis tight
hold off
pause;

a = -0.23; b = -2;
n = 5;
ttmp = linspace(-1, 11, n);
utmp = linspace(-5, 5, n);
vtmp = linspace(-5, 5, n);

[t, u, v] = meshgrid(ttmp, utmp, vtmp);
[tdx, udx, vdx] = ndgrid(ttmp, utmp, vtmp);


deltat = .01;


du = t*0 + deltat;
dv = deltat * (a*u + b*v + 0.2*cos(t));
dw = deltat * (-b*u + a*v + 0.5*sin(t));

dudx = tdx*0 + deltat;
dvdx = deltat * (a*udx + b*vdx + 0.2*cos(tdx));
dwdx = deltat * (-b*udx + a*vdx + 0.5*sin(tdx));

quiver3(t, u, v, du, dv, dw); axis equal


n_lines=20;

for i=1:n_lines
    x_rand(i)=rand;
end


xlabel('X')
ylabel('Y')
zlabel('Z')

h = streamline(t, u, v, du, dv, dw, x_rand', [-5:-5+n_lines-1]', zeros(n_lines, 1)'); set(h, 'color', [1,0,0])
set(h, 'linewidth', 2);

g = streamtube(t, u, v, du, dv, dw, x_rand', [-5:-5+n_lines-1]', zeros(n_lines, 1)')


datta = [tdx(:), udx(:), vdx(:)];                         
save -ascii streamlines3d datta



