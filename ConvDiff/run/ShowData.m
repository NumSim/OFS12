% Plotting grid

clear all

% load grid
MESHX=load('data.meshX');
MESHY=load('data.meshY');
ZEROS=zeros(size(MESHX(:,:)));
%PHI  =zeros(size(MESHX(:,:)));
PHI  =load('data.phi');

% show data  
%iso=subplot(1,2,1);
%   [C_str,h_str]=contour(MESHX,MESHY,PHI);
%   title(' isolines ');
%   set(iso,'DataAspectRatio',[1 1 1]);
%   hold on;
str=subplot(1,2,1:2);
   surf(MESHX,MESHY,ZEROS,PHI);
   title(' phi ');
   set(str,'DataAspectRatio',[1 1 1]); 
view(2);

%plottools('on');


