
T=load('cross.txt');
x=T(:,1);
y=T(:,2);
plot(x,y,'*');
%xlim([-1 10]); ylim([-1 10]);
title('schnittpunkte');
print('schnittpunkte.png');
