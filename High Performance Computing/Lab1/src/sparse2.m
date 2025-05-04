n = 5000;
A = spdiags(randn(n, 101), -50:50, n, n);
bs_max=50;
time=zeros(bs_max, 1);%preallocate to make things more consistent

for bs = 1:bs_max
 X = randn(n, bs);
 Y=zeros(n,bs);
 tic
 for loop=1:20
  Y=A*X;
 end
 time(bs)=toc;
end

hold off
figure(1)
plot(1:bs_max, time(1:bs_max)/time(1), 'r-', 'Linewidth', 2)
grid on

figure(2)
hold off
plot(1:bs_max, (time(1:bs_max)'./(1:bs_max)) /time(1), 'b-','Linewidth', 2)

hold on
x=[8,12];plot(x, (time(x)'./(x)) /time(1), 'ro','Linewidth', 2)
grid on
hold on