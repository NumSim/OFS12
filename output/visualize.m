function  visualize

%close all

hold off

Z=load('../output/phi.dat');
U=load('../output/u.dat');
V=load('../output/v.dat');
X=load('../output/grid.x.dat');
Y=load('../output/grid.y.dat');
E=load('../output/errorLog.dat');

numberOfPoints = 50;
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

Z = Z(1:Sz:ZX,1:Sz:ZY);
X = X(1:Sx:XX,1:Sx:XY);
Y = Y(1:Sy:YX,1:Sy:YY);
U = U(1:Su:UX,1:Su:UY);
V = V(1:Sv:VX,1:Sv:VY);


Z'

figure(1)
contourf(X,Y,Z,50)
xlabel('x');
ylabel('y');
zlabel('z');
title('isolines of potential');

figure(2)
quiver(X,Y,U,V);
title('velocity field');

figure(3)
P = -(U(2:end-1,2:end-1).^2+V(2:end-1,2:end-1).^2)/2*1;
contourf(X(2:end-1,2:end-1),Y(2:end-1,2:end-1),P,25);
title('pressure');

figure(4)
loglog(E)
title('error over iteration in 1000, log-log');


end

