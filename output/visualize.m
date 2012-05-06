function  visualize

%close all
clc
hold off

Z=load('../output/phi.dat');
U=load('../output/u.dat');
V=load('../output/v.dat');
X=load('../output/grid.x.dat');
Y=load('../output/grid.y.dat');
E=load('../output/errorLog.dat');

numberOfPoints = 100;
Sz = ceil(length(Z)/numberOfPoints);
Su = ceil(length(U)/numberOfPoints);
Sv = ceil(length(V)/numberOfPoints);
Sx = ceil(length(X)/numberOfPoints);
Sy = ceil(length(Y)/numberOfPoints);
[ZX,ZY] = size(Z);
[XX,XY] = size(X);
[YX,YY] = size(Y);
[UX,UY] = size(U);
[VX,VY] = size(V);

figure(23)
mesh(diff(X))
Z = Z(1:Sz:ZX,1:Sz:ZY);
X = X(1:Sx:XX,1:Sx:XY);
Y = Y(1:Sy:YX,1:Sy:YY);
U = U(1:Su:UX,1:Su:UY);
V = V(1:Sv:VX,1:Sv:VY);

% figure(1)
% mesh(U(2:end,2:end))
% title('U')
% xlabel('x')
% figure(2)
% mesh(V(2:end,2:end))
% title('V')
% xlabel('x')


figure(1)
contourf(X,Y,Z,40)
xlabel('x');
ylabel('y');
zlabel('z');
title('isolines of potential');
colorbar
hold on
size(diff(X))
mesh(X(2:end,1:end),Y(2:end,1:end),diff(X))


figure(2)

title('velocity field');
contourf(X(2:end-1,2:end-1),Y(2:end-1,2:end-1),U(2:end-1,2:end-1).^2+V(2:end-1,2:end-1).^2,20);
hold on
%quiver(X,Y,U,V);
hold off
colorbar
% 
% figure(3)
% P = -(U(2:end-1,2:end-1).^2+V(2:end-1,2:end-1).^2)/2*1; 
% contourf(X(2:end-1,2:end-1),Y(2:end-1,2:end-1),P,50);
% %contourf(P,55);
% title('pressure');
% colorbar

figure(4)
loglog(E)
title('error over iteration in 1000, log-log');
grid  on


end

