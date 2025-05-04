% So you get the same conditions in each test run.
clear

% Read jpg-image into matrix A.
A = imread('lena.jpg', 'jpg');

% Show the image.
figure(1)
image(A)
colormap(gray(235))
axis off image

% Convert matrix to double precision.
A = double(A);

% Form a kind of average image.
tic
  B = average(A);
toc

% Show the image. A and B look fairly similar, don't you think?
figure(2)
image(B)
colormap(gray(235))
axis off image

L = B - A;

% Plot nine BW-images with marked edges.
% L is kind of discretized Laplacian and we can use it to detect edges.
% There are more fancy edge detection algorithms, try for example
% the one used the image toolbox in Matlab:
% image(edge(A)); axis off image; colormap(gray(2))
% You can also try the algorithm used by the xv-command.
%
figure(3)
tic
  s = 1;
  for row = 1:3
    for col = 1:3
      subplot(3, 3, s)
      image(edges(L, s));
      colormap(gray(2)); % Only black and white.
      axis off image
      title(['s = ', num2str(s)])
      s = s + 1;
    end
  end
toc
