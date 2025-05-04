%Scientific Visualization
function yprim = f(t, y)
global P c

y2p = [0; 0; 0];

for n = 1:size(P,2) % loops over columns
    y2p = y2p + (y(1:3) - P(1:3, n)) / (norm( y(1:3) - P(1:3, n), 2))^3;    
end

yprim = [y(4:6); c*y2p]; %answer is 6 el.ts

%y(3:4)->y()
%{

y(1)       - Position r x
y(2)       - Position r y 
y(3)       - Position r z

yprim=
[
 y(4)      - Velocity r' i x-axis
 y(5)      - Velocity r' i y-axis
 y(6)      - Velocity r' i z-axis 
 c*y2p  - Acceleration (r'' [x; y; z])
]
%}