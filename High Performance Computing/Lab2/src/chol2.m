
n=600
A=eye(n)+1;


for k=1:n%k=1 k<n
  A(k, k) = sqrt(A(k, k) - sum(A(k, 1:k-1).^2));
  for i = k+1:n
    A(i, k) = (A(i, k) - sum(A(i, 1:k-1) .* A(k, 1:k-1))) / A(k, k);
  end
end

'res'
A(600,600)


