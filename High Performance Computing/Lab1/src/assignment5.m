n = 100000;
a = randn(n, 1);
b = a;
c = a;
loops = 200;


tic
  for j = 1:loops
    for k = 1:length(a)
      if a(k) < 0
        a(k) = 0;
      end
    end
  end
toc

% Using logical indices instead (b < 0 is an array of 1 and 0, true
% and false).
tic
  for j = 1:loops
    b(b < 0) = 0;
  end 
toc

% In some applications it is more useful to get the actual indices.

tic;
  for j = 1:loops
    indx = find(c < 0);
    c(indx) = 0;
  end
toc