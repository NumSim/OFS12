function  visualize

close all
clear all
clc

E = load('../output/errorLog.dat');
E= E(E~=0);
figure(222)
loglog(E);
grid on
xlabel('number of iteration');
ylabel('summed error in iteration');
title('Convergence for parallel Flow');


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


% X = linspace(1,2,20);
% Y = X;
% [X,Y] = meshgrid(X,Y);
% Z = 1*atan(X./Y);
% figure(33)
% contourf(X,Y,Z);
% Z(1,:)'-PHI(:,1)
% % Z(1,:)'
% % PHI(1,:)'
% % disp('###')
% % 
% % Z(:,1)
% % PHI(:,1)


 
figure(1)
contourf(X,Y,PHI,20)
title('Isolines of Phi for parallel Flow');
colorbar
xlabel('x');
ylabel('y');
zlabel('z');
figure(2)
contourf(X,Y,PSI,20)
title('Isolines of Psi for parallel Flow');
colorbar
xlabel('x');
ylabel('y');
zlabel('z');


figure(3)
contour(X,Y,PHI,20);
hold on
contour(X,Y,PSI,20);
title('Isolines of Phi and Psi for parallel Flow');
colorbar
xlabel('x');
ylabel('y');
