[re, im]    = meshgrid( linspace(-4, 1.5, 80), linspace(-6, 5, 80) ); 
Zrkf        = abs( rkf45(re+im.*i) );
Zdopri    = abs( dopri(re+im.*i) );


clf
whitebg('white');
hold off
figure(1)%not hold off after figure(1)

contour(re, im, Zdopri, [1, 1], 'k', 'LineWidth', 2);
%text(0.5, 3, '\leftarrow Zrkf', 'FontSize', 1)
%text(0, -2, '\leftarrow Zdopri', 'FontSize', 18)
hold on
contour(re, im, Zrkf, [1, 1], 'r', 'LineWidth', 2);
%contour3 specify a level
legend('Zdopri', 'Zrkf');
