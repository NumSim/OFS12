function  visualize

close all
clear all
clc

hold off

PHI=load('../output/phi.dat');
PSI=load('../output/psi.dat');
U=load('../output/u.dat');
V=load('../output/v.dat');
X=load('../output/grid.x.dat');
Y=load('../output/grid.y.dat');

numberOfPoints = 10;
Sz = ceil(length(PHI)/numberOfPoints);
Su = ceil(length(U)/numberOfPoints);
Sv = ceil(length(V)/numberOfPoints);
Sx = ceil(length(X)/numberOfPoints);
Sy = ceil(length(Y)/numberOfPoints);
[ZX,ZY] = size(PHI);
[XX,XY] = size(X);
[YX,YY] = size(Y);
[UX,UY] = size(U);
[VX,VY] = size(V);

PHI = PHI(1:Sz:ZX,1:Sz:ZY);
PSI = PSI(1:Sz:ZX,1:Sz:ZY);

X = X(1:Sx:XX,1:Sx:XY);
Y = Y(1:Sy:YX,1:Sy:YY);
U = U(1:Su:UX,1:Su:UY);
V = V(1:Sv:VX,1:Sv:VY);
 
 
% figure(1)
% mesh(X,Y,PHI)
% xlabel('x');
% ylabel('y');
% zlabel('z');
scale = 50;
figure('Position',[100,900,scale*(max(max(X))-min(min(X))),scale*(max(max(Y))-min(min(Y)))])
close 1

contour(X,Y,PHI,100);
hold on
contour(X,Y,PSI,50)

xlabel('x');
ylabel('y');
zlabel('z');

% figure(3)
%quiver(X,Y,U,V);


%figure(1)

% Y
% 
% 
% xlabel('x');
% ylabel('y');
% zlabel('z');

% figure(4)
% mesh(PHI)
% xlabel('x');
% ylabel('y');
% zlabel('z');






end

