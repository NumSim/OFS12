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



double xiof(double x,double y){

  return ((x-xleftof(x,y))/(xrightof(x,y)-xleftof(x,y)));

}
double etaof(double x, double y){
  return ((y-ybottomof(x,y))/(ytopof(x,y)-ybottomof(x,y)));

}
double xof(double xi,double eta){

  return (xi*(xrightof(xi,eta)-xleftof(xi,eta))+xleftof(xi,eta));
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
  return 1+xi;//4+cos(xi);
}
double ybottomof(double xi, double eta){
  return xi;//-sin(xi);
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
          dxidx[i][j] = (xiof(x+dx,y)-xiof(x-dx,y)) /twicedx;
          dxidy[i][j] = (xiof(x,y+dy)-xiof(x,y-dy)) /twicedy;
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
          ddxidx[i][j] = (xiof(x+dx,y)-2*xiof(x,y)+xiof(x-dx,y)) /dx2;
          ddxidy[i][j] = (xiof(x,y+dy)-2*xiof(x,y)+xiof(x,y-dy)) /dy2;
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
