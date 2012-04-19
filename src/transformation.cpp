/*
 * transformation.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: thomas
 */

#include <math.h>
#include <iostream>
#include "transformation.h"
#include "data.h"



double xiof(double x){
  double dummy=0;
  return ((x-xleftof(x,dummy))/(xrightof(x,dummy)-xleftof(x,dummy)));

}
double etaof(double x, double y){
  return ((y-ybottomof(x,y))/(ytopof(x,y)-ybottomof(x,y)));

}
double xof(double xi){
  double dummy=0;
  return (xi*(xrightof(xi,dummy)-xleftof(xi,dummy))+xleftof(xi,dummy));
}
double yof(double xi, double eta){
  return (eta*(ytopof(xi,eta)-ybottomof(xi,eta))+ybottomof(xi,eta));
}

double xleftof(double xi, double eta){
  return 0;
}
double xrightof(double xi, double eta){
  return 1;
}
double ytopof(double xi, double eta){
  return 4+cos(xi);
}
double ybottomof(double xi, double eta){
  return -sin(xi);
}



void dxi(sData* data, double** dxidx, double** dxidy){
  double dx = data->finiteDiffDx;
  double dy = data->finiteDiffDy;
  double twicedx = 2*dx;
  double twicedy = 2*dy;
  double x;
  double y;
  for (int i=1; i<data->dimI-1;i++){
      for (int j =1; j<data->dimJ-1;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          dxidx[i][j] = (xiof(x+dx)-xiof(x-dx)) /twicedx;
          dxidy[i][j] = (xiof(y+dy)-xiof(y-dy)) /twicedy;
      }
  }

}

void deta(sData* data, double** detadx, double** detady){
  double dx = data->finiteDiffDx;
  double dy = data->finiteDiffDy;
  double twicedx = 2*dx;
  double twicedy = 2*dy;
  double x;
  double y;
  for (int i=1; i<data->dimI-1;i++){
      for (int j =1; j<data->dimJ-1;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          detadx[i][j] = (etaof(x+dx,y)-etaof(x-dx,y)) /twicedx;
          detady[i][j] = (etaof(x,y+dy)-etaof(x,y-dy)) /twicedy;
      }
  }

}

void ddxi(sData* data, double** ddxidx, double** ddxidy){
  double dx = data->finiteDiffDx;
  double dy = data->finiteDiffDy;
  double dx2 = dx*dx;
  double dy2 = dy*dy;
  double x;
  double y;
  for (int i=1; i<data->dimI-1;i++){
      for (int j =1; j<data->dimJ-1;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          ddxidx[i][j] = (xiof(x+dx)-2*xiof(x)+xiof(x-dx)) /dx2;
          ddxidy[i][j] = (xiof(y+dy)-2*xiof(y)+xiof(y-dy)) /dy2;
      }
  }

}

void ddeta(sData* data, double** ddetadx, double** ddetady){
  double dx = data->finiteDiffDx;
  double dy = data->finiteDiffDy;
  double dx2 = dx*dx;
  double dy2 = dy*dy;
  double x;
  double y;
  for (int i=1; i<data->dimI-1;i++){
      for (int j =1; j<data->dimJ-1;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          ddetadx[i][j] = (etaof(x+dx,y)-2*etaof(x,y)+etaof(x-dx,y)) /dx2;
          ddetady[i][j] = (etaof(x,y+dy)-2*etaof(x,y)+etaof(x,y-dy)) /dy2;
      }
  }

}
