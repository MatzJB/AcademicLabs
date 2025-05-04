m = 2;
n = 100000;
A = sprandn(m, n, 0.001);
x = randn(n, 1);%1e5x1
y = zeros(m, 1);%2x1

% Note: you may NOT optimize the code by computing A * x once
% before the loop. Instead you should pretend that you get
% a new x-vector in each iteration and that the new x-vector
% depends on the previous one. You do not have access to all
% 1000 x-vectors when entering the loop, in other words.


tic
for k = 1:1000
  y = y + A * x;
end
toc
y_save = y;               % save y for later comparison


y = zeros(m, 1);%2x1

tic
%preprocessing, transpose makes for smaller variables
t=x';

A=A';
y=y';

for k = 1:1000
 y = y+t*A;%since Ax = (A'x')' = x'A'
end

y=y';
toc

error = norm(y - y_save)  % should be zero or very small
