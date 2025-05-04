
A = randn(1000);

tic
  for k = 1:10
    C = A' * A;
  end
toc


tic
  for k = 1:10
    C = A * A';
  end
toc


tic
  for k = 1:10
    C = A * A;
  end
toc


tic
  for k = 1:10
    C = (A' * A)';
  end
toc

%Elapsed time is 7.250000 seconds.
%Elapsed time is 8.110000 seconds.
%Elapsed time is 13.015000 seconds.

%Elapsed time is 0.328000 seconds.
%Elapsed time is 7.719000 seconds.


B=A';

tic
  for k = 1:10
    C = B * A;
  end
toc


