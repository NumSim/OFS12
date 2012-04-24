function  visualize

%close all

    hold off
    
Z=load('../output/scalar.dat');
X=load('../output/phyGrid.meshX');
Y=load('../output/phyGrid.meshY');

numberOfPoints = 10;
Sz = ceil(length(Z)/numberOfPoints);
Sx = ceil(length(X)/numberOfPoints);
Sy = ceil(length(Y)/numberOfPoints);
[ZX,ZY] = size(Z);  
[XX,XY] = size(X);
[YX,YY] = size(Y);
Z = Z(1:Sz:ZX,1:Sz:ZY);
X = X(1:Sx:XX,1:Sx:XY);
Y = Y(1:Sy:YX,1:Sy:YY);
whos
Z(end,:)
[U,V] = gradient(Z,X,Y);
 figure(1)
% mesh(U)
% figure(2)
% mesh(V)
% figure(3)
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

figure(4)
mesh(Z)
xlabel('x');
ylabel('y');
zlabel('z');

 figure(3)
 quiver(X,Y,U,V);
% figure(5)
% quiver(U,V);
%figure(5)
%streamline(X,Y,U,V,[1,2,3,4],[1,0,-1,2])
% 
% W = zeros(size(U));
% min(min(U))
% min(min(V))
% 
% tol = 1e-2;
% for i=1:length(U)
%     for j=1:length(U)
%         if abs(U(i,j))<tol && abs(V(i,j))<tol
%             W(i,j) = 1;
%         end
%     end
% end
% 
% surface(W)
% 
%     

% 
% POT = zeros(100,100);
% 
% for i=1:100
%     for j=1:100
%         POT(i,j) = i*0.01*1 + 1 * log((i*0.01-0.5)^2+(j*0.01-0.5)^2);
%     end
% end
% figure(4)
% mesh(POT)
% [W,Z] = gradient(POT);
% figure(5)
% mesh(W)
% figure(6)
% mesh(Z);






end