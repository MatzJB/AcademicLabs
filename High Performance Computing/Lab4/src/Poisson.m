% HPC LAB4 serial version of the lab

% x corresponds to j in U(j, k); to make the matrix elements match the
% 2D quadrant (visually), one must reverse the order of the y-values
% (U(i,1) = 1 - y(i))

% comments:
% * the code seems to work for n = 20 but not for n = 100
% * the errors and delta seem to differ between runs; at first I thought
% this was due to rounding errors, but according to the lab instructions, that shouldn't be the case
% the code works for n = 20 but not for n = 100 [Aug 23, 20:35]
% it works if you initialize U with zeros instead of random values //Thomas



function answer = HPC_lab4b_test()
%format short %better display
clc
display('Test av Jacobians (seriella) metod för lösning av PDE')
n = 100;
display( ['n=',num2str(n)] )
%format compact

visual=0;%om man vill rita i plottfönstret, kommer köra långsammare eftersom den uppdaterar fönstret (med en liten paus)

U_tilde = zeros(n+2); %matrix of u_tilde_{j,k} with a boundary
U = zeros(n+2); %rand(n+2); %matrix of u_{j,k}
F = U;%har också rand/kant (för att index skall bli detsamma)

h = 1/(n+1);%linspace?
t = 0:h:1;

[x, y] = meshgrid(t(2:n+1));%ritar upp allt utom randen

if(visual==1) 
    axis vis3d
    
    fig_residual = surf(x, y, x  , 'edgecolor', 'none', 'Facecolor', 'interp');
    fig_title = title('Testing Jacobi method','fontsize',13,'interp','tex');
    colormap jet(1024)
    axis manual
end

%==( Setting the boundary )==
%kontrollerat randen, är korrekt (20/5 ~08 ca: 16:15)

U(:, 1)     = g(t*0,   1-t); %y ändras (revers), vänstra randen
U(:, end)   = g(t*0+1, 1-t); %y ändras (revers), högra randen
U(1, :)     = g(t, t*0+1);   %x ändras, övre randen
U(end, :)   = g(t, t*0);     %x ändras, nedre randen

%är korrekt (kollad 20/5 ~08 16:30)
for i =2:n+1
    for j =2:n+1
        F(i, j) = f( (j-1)*h, 1-(i-1)*h );%matrisens index börjar på (0,1) så y koordinaterna måste ändras
    end
end
%ändrade index till j-1 (spelar ju roll faktiskt)

for i =1:n
    for j =1:n
        U_facit(i, j) = g( j*h, i*h );%test!!!
        %matrisens index börjar på (0,1) så y koordinaterna måste ändras
    end
end


Terror    = 20;
tau       = 1E-11;
iteration = 0;

%randen kommer att kommunicera med alla andra element i U genom k+1, j-1
while true
    iteration=iteration+1;
    %ändrat index så man inte får indexfel
    for j =2:n+1
        for k =2:n+1
            %U:[0,1]->R
            U_tilde(j, k) = 1/4*( U(j-1, k) + U(j+1, k) + U(j, k-1) + U(j, k+1) - h^2 * F(j, k) );
            %f vill ha argumenten (x_j, y_k) <- [0,1]x[0,1] loopen är förskjuten ett steg (för i=1:n -> i=2:n+1)
        end%for
    end%for

    delta = max( max( abs(U_tilde(2:n+1, 2:n+1) - U(2:n+1, 2:n+1)) ) );
    
    
    if(visual==1)
        set(fig_residual, 'zdata', abs(U_facit(1:n, 1:n) - U_tilde(2:n+1, 2:n+1)) )
        %set(fig_title, 'string', ['delta= ', num2str(delta,'%10.3E'),', error=', num2str('%10.3E',Terror)] )
        set(fig_title, 'string', ['n=',num2str(n),', \delta = ', num2str(delta),', Error=',num2str(Terror)] )
    end
        
    Terror = max(max( abs( U_facit(end:-1:1, :) - U(2:n+1,2:n+1) ) ));
    
    if mod(iteration, 100) == 0
        display([' delta = ', num2str(delta, '%10.2E'), '    Error = ', num2str(Terror, '%10.2E')])
    end
    
    if (visual==1)    
        pause(0.01)%annars hinner inte matlab plotta
    end
    
    if delta < tau
        if (visual==1)
            set(fig_residual, 'zdata', abs(U_facit - U_tilde(2:n+1, 2:n+1)) )
            %set(fig_title, 'string', ['\delta= ', num2str(delta,'%10.3E'),', error=', num2str('%10.3E',Terror)] )
            set(fig_title, 'string', ['n=',num2str(n),', \delta = ', num2str(delta),', Error=',num2str(Terror)] )
        end
        display(['...reached stopping condition (delta<tau), quitting...'])
        break
    end

    U(2:n+1, 2:n+1)     = U_tilde(2:n+1, 2:n+1);%randen skall inte ändras!!!
end%while

display([' delta = ', num2str(delta, '%10.2E'), '    Error = ', num2str(Terror, '%10.2E')])

%answer = U_tilde;

end


%Functions which works with vector arguments

function retn = f(x, y)
retn =  2*(cos(x + y) - (1 + x).*sin(x + y));
end

function retn = g(x, y)
retn =  (1 + x).*sin(x + y);
end
