
load arnoldi
whitebg('black');
%A = [dist]

A   = ones(size(ex, 1), 1);
%{
A(i) = min_j |ex(i)-app(j)|
%}

for i=1:size(ex)
    A(i) = min( abs(ex(i) - app) );
end

minR = min( real(ex) );
maxR = max( real(ex) );

ngrid = 2;

cdigits = -log10(A); %log10(1./A)==-log10(A)
cdigits = cdigits.*(cdigits>0);

ti      = minR:(minR+maxR)/n_steps:maxR;
%ti              = linspace(minR, maxR, ngrid);%:(minR+maxR)/n_steps:maxR;
[XI, YI]   = meshgrid(ti, ti);
ZI            = griddata(real(ex), imag(ex), cdigits, XI, YI, 'cubic');

clf
figure(1)%ny figur
hold off%rensa gammalt

xlabel('Real', 'Fontsize', 16, 'Fontweight', 'Bold')
ylabel('Imaginary', 'Fontsize', 16, 'Fontweight', 'Bold')
zlabel('# correct digits', 'Fontsize', 16, 'Fontweight', 'Bold')

set(gca, 'Fontsize', 16, 'Fontweight', 'Bold')

title('Eigenvalues visualization', 'Fontsize', 16, 'Fontweight', 'Bold')
hold on

c = surf(XI, YI, ZI, 'FaceColor', 'interp', 'EdgeColor', 'None');

colormap(jet);
colorbar

hold on
plot3(real(ex), imag(ex), cdigits, 'w.', 'Linewidth', 1)

zlim([0, max(cdigits)])