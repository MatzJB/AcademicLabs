function B = average(A)
%
% Form a new image, B, where each pixel in B is the average
% of the four neighbours in A. Pixels "outside" of A are assumed to
% be zero. The borders and corners of B are treated separately.
%

%
% Strictly inside the matrix.
%
[m, n] = size(A);
for k = 2:m-1
  for j = 2:n-1
    B(k, j) = 0.25 * (A(k-1, j) + A(k+1, j) + A(k, j-1) + A(k, j+1));
  end
end

%
% Borders
%
for j = 2:n-1
  B(1, j) = 0.25 * (A(2,   j) + A(1, j-1) + A(1, j+1));
  B(m, j) = 0.25 * (A(m-1, j) + A(m, j-1) + A(m, j+1));
end

for k = 2:m-1
  B(k, 1) = 0.25 * (A(k,   2) + A(k-1, 1) + A(k+1, 1));
  B(k, n) = 0.25 * (A(k, n-1) + A(k-1, n) + A(k+1, n));
end

%
% Corners
%
B(1, 1) = 0.25 * (A(1, 2)     + A(2, 1));
B(1, n) = 0.25 * (A(1, n - 1) + A(2, n));
B(m, 1) = 0.25 * (A(m - 1, 1) + A(m, 2));
B(m, n) = 0.25 * (A(m, n - 1) + A(m - 1, n));

