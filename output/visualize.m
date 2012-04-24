function  visualize

%close all

hold off

Z=load('../output/phi.dat');
U=load('../output/u.dat');
V=load('../output/v.dat');
X=load('../output/phyGrid.meshX');
Y=load('../output/phyGrid.meshY');

numberOfPoints = 10;
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
U = U(1:Su:UX,1:Su:VY);
V = V(1:Sv:VX,1:Sv:VY);


figure(1)
Z(1,:)
Z(:,1)
Z(:,end)
max(max(Z))
mesh(X,Y,Z)
xlabel('x');
ylabel('y');
zlabel('z');

figure(2)
contourf(X,Y,Z,50)
xlabel('x');
ylabel('y');
zlabel('z');

figure(3)
quiver(X,Y,U,V);

figure(4)
mesh(Z)
xlabel('x');
ylabel('y');
zlabel('z');






end

