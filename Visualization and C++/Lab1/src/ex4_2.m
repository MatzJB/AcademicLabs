%3 real roots, all same, should be a curve
hold off
whitebg('white')
title('Two 3d problems', 'Fontsize', 16, 'Fontweight', 'Bold')

hold on
xlabel('A', 'Fontsize', 16, 'Fontweight', 'Bold')
ylabel('B', 'Fontsize', 16, 'Fontweight', 'Bold')
zlabel('C', 'Fontsize', 16, 'Fontweight', 'Bold')

r = linspace(-2.5, 2.5, 45);
A = -3.*r; B = 3.*r.^2; C = -r.^3;

plot3(A, B, C , '-', 'lineWidth', 2,'color',[.7, .2, 0.1]);

% 3 real roots 2 same:
colormap(hsv)
n=140;
% they could be different since the mapping is done
tmpx = linspace(-3, 3, n);
tmpy = linspace(-3, 3, n);
[s, t] = meshgrid(tmpx, tmpy);%roots s, t 
hold on
%A = -t; B = s.^2; C = t.*s.^2;
%A = -t; B = s.^2; C = -t.*s.^2;
%t=r3
%s=r1
%A = -t-2.*s; B = s.^2+s.*t; C = s.^2.*t;
A = -t-2.*s; B = 2.*t.*s+s.^2; C = -s.^2.*t;

%mesh(A,B,C)
c = surf(A, B, C, 'FaceColor', 'interp', 'EdgeColor', 'None', 'faceAlpha', 0.5);%, 'FaceColor', 'interp', 'edgeColor', 'none');
axis equal


%light('Position',[1 0 2],'Style','local')
%light('Position',[0 0 1],'Style','infinite')

%Teori:
%2: (x-s)^2(x-t) surface
%1: (x-s)^3 a curve
%The curve is a subset of the surface
%surface, where s==t
axis tight
