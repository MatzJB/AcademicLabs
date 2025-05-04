function E = edges(L, s)

E=zeros(size(L));

for j = 1:size(L, 1)
  for k = 1:size(L, 2)
    if abs(L(j, k)) < s
     E(j, k) = 2;
    else
     E(j, k) = 1;
    end
 end
end


%for j = 1:size(L, 1)
%  for k = 1:size(L, 2)
%      E(j, k)= (L(j, k)*L(j, k) < s*s)+1;
%  end
%end

