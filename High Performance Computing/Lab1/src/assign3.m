n = 100000;
x = 0.5 - rand(n, 1);
y = 0.5 - rand(n, 1);
loops = 1000;

tic
  for j = 1:loops
    for k = 1:n
      y(k) = 1.2345e-3 * y(k) + 2.2e-3 * x(k) + x(k) * y(k);
    end
  end
toc

tic
  for j = 1:loops
    y = 1.2345e-3 * y + 2.2e-3 * x + x .* y;
  end
toc
