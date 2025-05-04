n = 1000;
p = 500;
x = randn(n, 1);

tic
  X = [];
  for k = 1:p
    X(:, k) = x;
  end
toc

% In MATLAB, memory is allocated in a heap column-wise consecutively. For each loop
% a heap of size n, 2*n, 3*n, etc., is allocated... because you assign X using
% X(:,1), X(:,2),... and #x = n


tic
  X = zeros(n, p);
  for k = 1:p
    X(:, k) = x;
  end
toc

% Allocation happens directly, initializing X to zeros first (takes very little time in comparison)


tic
  X = [];
  for k = p:-1:1
    X(:, k) = x;
  end
toc

% We allocate the matrix directly, because the first call is x(:,500), so
% a heap of that size is allocated, and no more allocations are needed since
% the heap was made large enough from the start.



n = 30000;
m = 150;
tic
  X = sparse(n, n);
  for j = 1:m:n
    for k = 1:m:n
      X(j, k) = 1;
    end
  end
toc

% Suppose we know the sparsity structure
[rows, cols] = find(X);

tic
  X = sparse(rows, cols, ones(size(rows)));
  for j = 1:m:n
    for k = 1:m:n
      %X(j, k) = 1;  % but not the elements
    end
  end
toc



tic
  X = sparse(rows, cols, ones(size(rows)));
toc



tic
  X = sparse(n, n);
  for k = 1:m:n
    for j = 1:m:n
      X(j, k) = 1;
    end
  end
toc
