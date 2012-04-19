function  visualize
close all


    
Z=load('../output/scalar.dat');
X=load('../output/phyGrid.meshX');
Y=load('../output/phyGrid.meshY');
numberOfPoints = 5;
Sz = ceil(length(Z)/numberOfPoints);
Sx = ceil(length(Z)/numberOfPoints);
Sy = ceil(length(Z)/numberOfPoints);
Z = Z(1:Sz:length(Z),1:Sz:length(Z));
X = X(1:Sx:length(X),1:Sx:length(X));
Y = Y(1:Sy:length(Y),1:Sy:length(Y));
whos
[U,V] = gradient(Z);
% figure(1)
% mesh(U)
% figure(2)
% mesh(V)
% figure(3)
mesh(X,Y,Z)
figure(4)
quiver(X,Y,U,V);
figure(5)
quiver(U,V);
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