%{
x = 0:0.1:5*pi; X = cos(x)'; Y = sin(x)'; Z= x';
tube4([X,Y,Z], 0.1, (Z)*[1,1,1,1])
%}
%{
x = 0:0.1:5*pi; X = cos(x)'; Y = sin(x)'; Z= x';
>> surf([X X],[Y Y],[Z Z+1])
%}

function w = tube4(pos, rad, color)% ra is an int, color is vector [r g b]


pos_s = size(pos, 1);
%{
if ~(pos_s==size(color, 1) | pos_s==1) 
   display('size of color and size of pos does not match');
end
%}
x = pos(:, 1);
y = pos(:, 2);
z = pos(:, 3);
alpha=1;
c = color;

e=surf( [x x], [y y], [z-rad/2 z+rad/2]);%, 'FaceColor', 'interp', 'EdgeColor', 'None', 'FaceAlpha', alpha);
set(e,  'FaceAlpha', 0.6);
set(e, 'FaceLighting', 'phong', 'Backfacelighting', 'reverselit', 'Edgecolor', 'none');
set(e, 'SpecularColorReflectance', 0.6, 'SpecularStrength', 1, 'SpecularExponent', 10);%'diffus' komponent
w=1;