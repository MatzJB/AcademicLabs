global P c
c   = 0.001;

% P contains the coordinates for the fixed particles. One particle per column.
P = [
 -0.0466  0.0368  0.1156 -0.1553 -0.1079  0.1564  0.1155  0.1430  0.0826 0.1333
 -0.0516 -0.0825 -0.0364 -0.1542  0.0528 -0.1747  0.0316  0.0713  0.1680 0.0453
  0.1293  0.0281 -0.1349 -0.1852  0.1727 -0.0044 -0.1318 -0.1272 -0.1713 0.0056];

whitebg('black')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% lighting
lights=[-3, -3, -3;
                2, 2, 1];
light('Position', lights(1, :), 'Style', 'local')
%light('Position', lights(2, :), 'Style', 'local')

plot3(lights(:, 1), lights(:, 2), lights(:, 3), 'y*', 'lineWidth', 4)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


xlabel('x', 'Fontsize', 16, 'Fontweight', 'Bold')
ylabel('y', 'Fontsize', 16, 'Fontweight', 'Bold')
zlabel('z', 'Fontsize', 16, 'Fontweight', 'Bold')

title('Scattering', 'Fontsize', 16, 'Fontweight', 'Bold')

tspan = linspace(0, 15, 50); 
phi_s = linspace(0, 2*pi, 8);

% Find the trajectories for a few particles...
for R = [0.1 0.2 0.3]  % R is the radius, and not r
    for phi = phi_s(1:end-1)
      [t, y] = ode45('f', tspan, [2 R*cos(phi) R*sin(phi) -0.3 0 0]');
      hold on
        tube4( y(:, 1:3), R, [y(:, 4:6)]);%y(4).*[R,R,R,R]);%abs( [y(:, 4:6), y(:, 4)*0+0.8] ));
  end
end

[xa, ya, za]   = sphere(13);%usage of y, hence name ya
rad = 0.05;
for w=1:size(P, 2)
    %hold on
    d = surf(rad*xa+P(1, w), rad*ya+P(2, w), rad*za+P(3, w));%, 'edgeColor', 'none');%hantaget fungerar inte?
    set(d, 'FaceColor', [rand,0.5*rand,rand]);
    set(d, 'FaceLighting', 'phong', 'Backfacelighting', 'reverselit', 'Edgecolor', 'none');
    set(d, 'SpecularColorReflectance', 0.6, 'SpecularStrength', 1, 'SpecularExponent', 15);
end
%shading interp

axis equal