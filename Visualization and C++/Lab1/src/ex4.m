%[x y;y z]
%a 2x2 matrix is represented in R^3 as [x y;y z]
%A surface of a singular matrix: 
clf
figure(1)

whitebg('black');
Title('Two 3D-problems a) [x y;y z]', 'Fontsize', 16, 'Fontweight', 'Bold')
I = eye(2);
hold on, plot3(I(1,1), I(2,1), I(2,2), 'm.', 'LineWidth', 3);

m = [1 0; 0 0.01];
hold on, plot3(m(1,1), m(2,1), m(2,2), 'go', 'LineWidth', 1);

s = [1 0;0 0];
hold on, plot3(m(1,1), m(2,1), m(2,2), 'r.', 'LineWidth', 3);

h = legend('Identity', 'Large \kappa', 'Singular', 3);
set(h, 'FontSize', 14, 'Location', 'NorthEastOutside')
%Text('Kanji!', 'Location', 'NorthEastOutside')
%{
[X, Z]  = meshgrid(linspace(0, 5, 50), linspace(0, 5, 50));%meshgrid                            
Y       = sqrt(X.*Z);      
hold on
surf(X, Y, Z, 'FaceColor', 'interp', 'FaceLighting', 'phong', 'EdgeColor', 'None')
hold on
surf(X, -Y, Z, 'FaceColor', 'interp', 'FaceLighting', 'phong', 'EdgeColor', 'None')


[X, Z]  = meshgrid(linspace(-5, 0, 50), linspace(-5, 0, 50));%meshgrid 
Y       = sqrt(X.*Z);   
hold on
surf(X, Y, Z, 'FaceColor', 'interp', 'FaceLighting', 'phong', 'EdgeColor', 'None')
hold on
surf(X, -Y, Z, 'FaceColor', 'interp', 'FaceLighting', 'phong', 'EdgeColor', 'None')
%}

%or
alpha   = 0.7;
gridn   = 70;
[Xpos, Zpos] = meshgrid(linspace(0, 5, gridn), linspace(0, 5, gridn));
[Xneg, Zneg] = meshgrid(linspace(-5, 0, gridn), linspace(-5, 0, gridn));
Ypos2        = sqrt(Xpos.*Zpos);
Yneg2        = sqrt(Xneg.*Zneg);

for i=-1:2:1
    hold on, surf(Xpos, i*Ypos2, Zpos, 'FaceColor', 'interp', 'EdgeColor', 'None', 'FaceAlpha', alpha)
    hold on, surf(Xneg, i*Yneg2, Zneg, 'FaceColor', 'interp', 'EdgeColor', 'None', 'FaceAlpha', alpha)
end

colormap(copper)
axis square

%light('Position',[0 1 0],'Style','infinite')
%light('Position',[0 0 1],'Style','infinite')

xlabel('x', 'Fontsize', 16, 'Fontweight', 'Bold')
ylabel('y', 'Fontsize', 16, 'Fontweight', 'Bold')
zlabel('z', 'Fontsize', 16, 'Fontweight', 'Bold')
grid
