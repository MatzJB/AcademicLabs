

>>a=1.0/0.0, b=1.0/a, sina=sin(a), sinb=sin(b), loga=log(a), logb=log(b)

Warning: Divide by zero.

a =

   Inf

Hugs> 1.0/0.0
1.#INF

b =

     0
   
     
Hugs> let a=1.0/0.0 in 1.0/a
0.0
  
     
     
sina =

   NaN

Hugs> let a=1.0/0.0 in sin(a)
-1.#IND

   

sinb =

     0
     
Hugs> let a=1.0/0.0; b=1.0/a in sin(b)
0.0


loga =

   Inf

   
Hugs> let a=1.0/0.0; b=1.0/a in sin(b)
0.0
   

Warning: Log of zero.

logb =

  -Inf
  
Hugs> let a=1.0/0.0; b=1.0/a in log(b)

Program error: argument out of range


>>c=0.0/0.0, d=1.0/c, sin(d)
  
  
  Warning: Divide by zero.
 
c =

   NaN

Hugs> 0.0/0.0
-1.#IND
   

d =

   NaN

Hugs> let c=0.0/0.0 in 1.0/c
-1.#IND
   
sind =

   NaN
  
Hugs> let c=0.0/0.0; d=1.0/c in sin(d)
-1.#IND


   
.......................................
    

>> delta=1E-15; x=2;(x + delta) - x

ans =

   8.8818e-16

>> delta=1E-16; x=2;(x + delta) - x

ans =

     0
     
     delta=1E6; x=2E20;(x + delta) - x

ans =

     1015808

>> delta=1E4; x=2E20;(x + delta) - x

ans =

     0
     
   
>> delta=1E-36; x=2E-20;(x + delta) - x

ans =

     0

>> delta=1E-35; x=2E-20;(x + delta) - x

ans =

   9.0278e-36

   
  
  ................................ 


>> s2=single(1.0./([1E6:-1:1]));
>> s1=single(1.0./[1:1E6]);
   
   
>> s1=sum(s1)

s1 =

  14.3573580

>> s2=sum(s2)

s2 =

  14.3926516
   
  
  sum(abs(s1-sort(s2,2,'descend'))<1e-4)
  

  1E6-43
  
  sum [1/x|x<-[1..1000000]]
  
  ................................
      


-LU faktorisering-


n = 3000;
A = randn(n);
b = randn(n, 1);
tic; x = inv(A) * b; toc
tic; y = A \ b; toc

Elapsed time is 27.151693 seconds.
Elapsed time is 9.216649 seconds.


\ more efficient than to take the inverse of A

..................................



n = 3000;
e = ones(n, 1);
A = spdiags([e 2*e e], -1:1, n, n);
spy(A)
b = randn(n, 1);
tic; x = inv(A) * b; toc
tic; y = A \ b; toc
Elapsed time is 1.542246 seconds.
Elapsed time is 0.000366 seconds.


>> nnz(A)

ans =

        8998

>> nnz(A^-1)

ans =

     9000000

>> nnz(chol(A))

ans =

        5999


-memory-

Assume a floating point number require 8 bytes
and each 4 bytes for index => 16*n bytes, n=numer of elements

A:

8998*16 = 143,968B ~ 144KB 
        
chol(A):  
5,999*16 = 95,984 ~ 96KB


inv(A):
9000000*16 = 1.44E8 ~ 144MB

...................................    
A=hilb(10)

A =

  Columns 1 through 9 

    1.0000    0.5000    0.3333    0.2500    0.2000    0.1667    0.1429    0.1250    0.1111
    0.5000    0.3333    0.2500    0.2000    0.1667    0.1429    0.1250    0.1111    0.1000
    0.3333    0.2500    0.2000    0.1667    0.1429    0.1250    0.1111    0.1000    0.0909
    0.2500    0.2000    0.1667    0.1429    0.1250    0.1111    0.1000    0.0909    0.0833
    0.2000    0.1667    0.1429    0.1250    0.1111    0.1000    0.0909    0.0833    0.0769
    0.1667    0.1429    0.1250    0.1111    0.1000    0.0909    0.0833    0.0769    0.0714
    0.1429    0.1250    0.1111    0.1000    0.0909    0.0833    0.0769    0.0714    0.0667
    0.1250    0.1111    0.1000    0.0909    0.0833    0.0769    0.0714    0.0667    0.0625
    0.1111    0.1000    0.0909    0.0833    0.0769    0.0714    0.0667    0.0625    0.0588
    0.1000    0.0909    0.0833    0.0769    0.0714    0.0667    0.0625    0.0588    0.0556

  Column 10 

    0.1000
    0.0909
    0.0833
    0.0769
    0.0714
    0.0667
    0.0625
    0.0588
    0.0556
    0.0526

x=ones(10,1)


x =

     1
     1
     1
     1
     1
     1
     1
     1
     1
     1

b=A*x

b =

    2.9290
    2.0199
    1.6032
    1.3468
    1.1682
    1.0349
    0.9307
    0.8467
    0.7773
    0.7188

    
>> format long e
>> A\b

ans =

     9.999999988995500e-01
     1.000000094005028e+00
     9.999980154623497e-01
     1.000017912363931e+00
     9.999150675502133e-01
     1.000232312043433e+00
     9.996204783038589e-01
     1.000365402573885e+00
     9.998087904538157e-01
     1.000041929041141e+00
     
     
r = A*x - b

r =

     4.440892098500626e-16
                         0
                         0
                         0
                         0
                         0
    -1.110223024625157e-16
                         0
    -1.110223024625157e-16
                         0

                         
                         
                         