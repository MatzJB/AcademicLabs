
%creates a sparse bandmatrix from columns of col places them by the diagonals specified by diag
%-1:1 is then -1 0 1 indices in the diagonal
%-1:2 would be a superdiagonal of width 2 and subdiagonal of width 1
%n=10; e=ones(n,1); A=spdiags([e,e,e],[0,1,2],n,n)

%ones(n,w+1)%is used as diagonal for super, sub and diagonal vectors
S=zeros(n);
n = 3000
x=rand(n, 1);
y=zeros(n, 1);

%if(~(floor(w)==ceil(w))) error('change w, it is creating an offset band matrix'); end

start=10; leap=10; nloops=10; t=1; %time index
A=ones(n);

tic
for loop=1:nloops
 y=A*x;
end

t_full=toc;

for width = start:leap:n
 A = spdiags(ones(n, 2*width+1), floor(-width:width), n, n); %w is the width of super and sub

 tic
 for loop = 1:nloops
  y=A*x;
 end
 
 S(t) = toc;
 t = t+1 %write it with start+t*leap

 if (S(t)>t_full)
  disp('Sparse time exceeds full matrix time!');
 end
end%for

hold off
figure(1)
hold on;
xlabel('Width of sub', 'Fontsize', 14, 'Fontweight', 'Bold');
ylabel('Time (seconds)', 'Fontsize', 14, 'Fontweight', 'Bold');
set(gca, 'Fontsize', 14, 'Fontweight', 'Bold')

%to show the x-axis in width with vector [1, t-1] where t-1 is index of last
%time recorded in S
plot(start+([1, t-1]-1)*leap, [t_full, t_full], 'r-', 'Linewidth', 2);
plot(start+((1:(t-1))-1)*leap, S(1:t-1), 'b-','Linewidth', 2);
%real width == 2*(start + ( 1:(t-1) -1)*leap)+1 == 2*width+1

title('Band Matrix vs. Dense Matrix', 'Fontsize', 14, 'Fontweight', 'Bold');
legend('Dense Matrix', 'Band Matrix');

grid on
hold on