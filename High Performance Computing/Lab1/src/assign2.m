n = 1000;
v = randn(n, 1);
a = randn(n, 1);
loops = 100;

tic
  for j = 1:loops
    A = v * v';
    y = A * a;
  end
toc

tic
  for j = 1:loops
    y = v * v' * a;
  end
toc

tic
  for j = 1:loops
    y = v * (v' * a);
  end
toc

% loop 1: Evaluates due to operator precedence first v*v', which gives a matrix
% that’s n*n additions and n*n multiplications, i.e., O(n^2)

% We allocate A and copy the matrix into A, then matrix * column vector -> column vector,
% that's n*n operations. O(n^2), so in total O(n^2) *,+

% additional additions
% loop 2: same as above but no allocation to a temporary variable A
% loop 3: evaluates first a scalar (n ops), then vector * scalar = n ops, gives O(n + n)
% = O(n)
