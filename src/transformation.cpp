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
  return 20;//+10*eta;//+sin(5*eta);//+5*eta+sin(10*eta);;//+cos(eta);
}
double ytopof(double xi, double eta){


  if (xi<3){
      return 5;
  }else{
      return 4+cos(xi-3);
  }


 // return cos(xi)+9;
/*
  if (xi<3){
      return 8;
  }else if(xi<15){
      return 8+xi-3;
  }else{
      return 20;
  }

  return 1+xi;*/
/*
  if (xi<10){
      return 1+xi;
  }else{
      return 21-xi;
  }
*/
  //return 1+xi*xi;//+xi;//xi*xi+2+xi;
  //return 10+(xi);//50+xi-5.0*sin(xi);//4+cos(xi);
}
double ybottomof(double xi, double eta){
//  return cos(xi);
  //return 10*xi;//sin(xi);//-0.05*xi
  if (xi<3){
      return 1;
  }else{
      return cos(xi-3);
  }


/*
  if (xi<10){
      return -xi;
  }else{
      return -20+xi;
  }
*/
/*
 if (xi<3){
      return 0;
  }else if(xi<15){
      return 0+xi-3;
  }else{
      return 12;
  }*
*/
 // return xi;;//xi;//xi*xi+0.5*sin(xi)-4*xi;
  //return (xi);
  //return (xi<5 ? 0 : -1);//-xi*xi/50.0+0.1*sin(5.0*xi);//-sin(xi);
}

double ybottomDxof(double x, double y){
  double dx =1e-4;
  return (ybottomof(x+dx,y)-ybottomof(x-dx,y))/2/dx;


}

double ytopDxof(double x, double y){
  double dx =1e-4;
  return (ytopof(x+dx,y)-ytopof(x-dx,y))/2/dx;

}



void dxi(sData* data, double** dxidx, double** dxidy){
  double dx = data->finiteDiffDx;
  double dy = data->finiteDiffDy;
  double twicedx = 2.*dx;
  double twicedy = 2.*dy;
  double x;
  double y;
  //double xi;
  //double eta;
  for (int i=0; i<data->dimI;i++){
      for (int j =0; j<data->dimJ;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          //xi = data->xi[i][j];
          //eta = data->eta[i][j];
          dxidx[i][j] = (xiof(x+dx,y)-xiof(x-dx,y)) /twicedx;
          dxidy[i][j] = (xiof(x,y+dy)-xiof(x,y-dy)) /twicedy;
      }
  }

}

void deta(sData* data, double** detadx, double** detady){
  double dx = data->finiteDiffDx;
  double dy = data->finiteDiffDy;
  double twicedx = 2.*dx;
  double twicedy = 2.*dy;
  double x;
  double y;
  //double xi;
  //double eta;
  for (int i=0; i<data->dimI;i++){
      for (int j =0; j<data->dimJ;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          //xi = data->xi[i][j];
          //eta = data->eta[i][j];
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
  ///double xi;
  //double eta;
  for (int i=0; i<data->dimI;i++){
      for (int j =0; j<data->dimJ;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          //xi = data->xi[i][j];
          //eta = data->eta[i][j];
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
  //double xi;
  //double eta;
  for (int i=0; i<data->dimI;i++){
      for (int j =0; j<data->dimJ;j++){
          x = data->x[i][j];
          y = data->y[i][j];
          //xi = data->xi[i][j];
          //eta = data->eta[i][j];
          ddetadx[i][j] = (etaof(x+dx,y)-2*etaof(x,y)+etaof(x-dx,y)) /dx2;
          ddetady[i][j] = (etaof(x,y+dy)-2*etaof(x,y)+etaof(x,y-dy)) /dy2;
      }
  }

}
