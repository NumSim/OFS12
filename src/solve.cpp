/***************************************************************************
 *   Copyright (C) 2006-2011 by  Institute of Combustion Technology        *
 *   jens.henrik.goebbert@itv.rwth-aachen.de                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <math.h>
#include <iostream>

#include "solve.h"
#include "data.h"
#include "setup.h"
#include "transformation.h"

float fAbs(float x)
{
  return x>0 ? x : -x;
}
//------------------------------------------------------
bool solve(sData* data)
{
  std::cout << "\nSolve:\t------->\t";

  if(!gaussseidelMorphed(data,data->s1)){ return false; }
  //if(!gaussseidel(data,data->s1)){ return false; }
  //if(!jacobi(data, data->s1)) { return false; }
  //if(!thomas(data,data->s1))r { return false; }
  std::cout << "Success!\n";
  return true;
}

//------------------------------------------------------


bool gaussseidelMorphed(sData* data, double** s)
{
  int curIter=0;
  int N=data->dimI-2;
  int M=data->dimJ-2;
  double error;
  float tmp;
  double a1,a2,a3,a4,a5;
  double dxi1, dxi2, deta1,deta2;
  dxi1 = data->deltaXi;
  deta1 = data->deltaEta;
  dxi2 = dxi1*dxi1;
  deta2= deta1*deta1;


  // allocate memory for derivatives
  double ***alpha = new double**[N+2];
  double **dxiDx = new double*[N+2];
  double **dxiDy = new double*[N+2];
  double **detaDx = new double*[N+2];
  double **detaDy = new double*[N+2];
  double **ddxiDDx = new double*[N+2];
  double **ddxiDDy = new double*[N+2];
  double **ddetaDDx = new double*[N+2];
  double **ddetaDDy = new double*[N+2];
  // for wall
  double **beta = new double*[N+2];
  double **dhDx = new double*[N+2];

  for (int i=0;i<N+2;i++){
      dxiDx[i] = new double[M+2];
      dxiDy[i] = new double[M+2];
      detaDx[i] = new double[M+2];
      detaDy[i] = new double[M+2];
      ddxiDDx[i] = new double[M+2];
      ddxiDDy[i] = new double[M+2];
      ddetaDDx[i] = new double[M+2];
      ddetaDDy[i] = new double[M+2];
      alpha[i] = new double* [M+2];
      beta[i] = new double[2];
      dhDx[i] = new double[2];
  }
  for (int i=0;i<N+2;i++){
      for(int j=0;j<M+2;j++){
          alpha[i][j] = new double[5];
      }
  }



  // Calculate derivatives
  dxi(data,dxiDx,dxiDy);
  deta(data,detaDx,detaDy);
  ddxi(data,ddxiDDx,ddxiDDy);
  ddeta(data,ddetaDDx,ddetaDDy);
  for (int i=0;i<N+2;i++){
      dhDx[i][0] = (ybottomof(data->x[i][0]+data->finiteDiffDx,data->y[i][0]) -ybottomof(data->x[i][0]-data->finiteDiffDx,data->y[i][0])      )/(2*data->finiteDiffDx);
      dhDx[i][1] = (ytopof(data->x[i][data->dimJ-1]+data->finiteDiffDx,data->y[i][data->dimJ-1]) -ytopof(data->x[i][data->dimJ-1]-data->finiteDiffDx,data->y[i][data->dimJ-1])      )/(2*data->finiteDiffDx);
  }



  // Calculate alpha for inside
  for (int i=1;i<data->dimI-1;i++){
      for(int j=1;j<data->dimJ-1;j++){
          alpha[i][j][0] = (dxiDx[i][j]*dxiDx[i][j]+dxiDy[i][j]*dxiDy[i][j] )/dxi2;
          alpha[i][j][1] = (detaDx[i][j]*detaDx[i][j]+detaDy[i][j]*detaDy[i][j] )/deta2;
          alpha[i][j][2] = 2.*(dxiDx[i][j]*detaDx[i][j]+dxiDy[i][j]*detaDy[i][j])/deta1/dxi1;
          alpha[i][j][3] = (ddxiDDx[i][j]+ddxiDDy[i][j])/dxi1;
          alpha[i][j][4] = (ddetaDDx[i][j]+ddetaDDy[i][j])/deta1;
      }
  }

  // Calculate beta for wall
  for (int i=1;i<data->dimI-1;i++){
      beta[i][0] = (dhDx[i][0]*dxiDx[i][0]-dxiDy[i][0])/(dhDx[i][0]*detaDx[i][0]-detaDy[i][0])* deta1/dxi1;
      beta[i][1] = (dhDx[i][1]*dxiDx[i][data->dimJ-1]-dxiDy[i][data->dimJ-1)/(dhDx[i][1]*detaDx[i][data->dimJ-1]-detaDy[i][data->dimJ-1])* deta1/dxi1;
  }

  // free memory
  for (int i=0;i<N+2;i++){
      delete[] dxiDx[i];
      delete[] dxiDy[i];
      delete[] detaDx[i];
      delete[] detaDy[i];
      delete[] ddxiDDx[i];
      delete[] ddxiDDy[i];
      delete[] ddetaDDx[i];
      delete[] ddetaDDy[i];
      delete[] dhDx[i];
  }
  delete[] dxiDx;
  delete[] dxiDy;
  delete[] detaDx;
  delete[] detaDy;
  delete[] ddxiDDx;
  delete[] ddxiDDy;
  delete[] ddetaDDx;
  delete[] ddetaDDy;
  delete[] dhDx;



  error = 0;
  while(curIter<data->maxIter) {
      std::cout << "\r\tGauss-Seidel: Iteration " << ++curIter << ", Residual= " << error;
      error =0;
      for(int i = 1; i < data->dimI-1; i++)
        {
          if ((i%2)==0 ){ // upwards
              s[i][0] = (   beta[i][0]*(s[i+1][0]-s[i-1][0]) - s[i][2] + 4*s[i][1]     )/3;
              for(int j = 1 ; j < data->dimJ-1; j++)
                {
                  a1 = alpha[i][j][0];
                  a2 = alpha[i][j][1];
                  a3 = alpha[i][j][2];
                  a4 = alpha[i][j][3];
                  a5 = alpha[i][j][4];

                  tmp =        s[i+1][j+1]   * (a3/4.0)
                                                         + s[i+1][j]     * (a1+a4/2.0)
                                                         + s[i+1][j-1]   * (-a3/4.0)
                                                         + s[i][j+1]     * (a2+a5/2.0)
                                                         + s[i][j-1]     * (a2-a5/2.0)
                                                         + s[i-1][j+1]   * (-a3/4.0)
                                                         + s[i-1][j]     * (a1-a4/2.0)
                                                         + s[i-1][j-1]   * (a3/4.0);
                  tmp /=(2.0*(a1+a2));
                  error += fAbs(tmp-s[i][j]);
                  s[i][j] = tmp;
                }
              s[i][data->dimJ-1] = (   - beta[i][1]*(s[i+1][data->dimJ-1]-s[i-1][data->dimJ-1]) - s[i][data->dimJ-1-2] + 4*s[i][data->dimJ-1-1] )/3;
          }
          else{ //downwards
              s[i][data->dimJ-1] = (   - beta[i][1]*(s[i+1][data->dimJ-1]-s[i-1][data->dimJ-1]) - s[i][data->dimJ-1-2] + 4*s[i][data->dimJ-1-1] )/3;
              for(int j = data->dimJ-2 ; j >0; j--)
                {
                  a1 = alpha[i][j][0];
                  a2 = alpha[i][j][1];
                  a3 = alpha[i][j][2];
                  a4 = alpha[i][j][3];
                  a5 = alpha[i][j][4];

                  tmp =        s[i+1][j+1]   * (a3/4.0)
                                                         + s[i+1][j]     * (a1+a4/2.0)
                                                         + s[i+1][j-1]   * (-a3/4.0)
                                                         + s[i][j+1]     * (a2+a5/2.0)
                                                         + s[i][j-1]     * (a2-a5/2.0)
                                                         + s[i-1][j+1]   * (-a3/4.0)
                                                         + s[i-1][j]     * (a1-a4/2.0)
                                                         + s[i-1][j-1]   * (a3/4.0);
                  tmp /=(2.0*(a1+a2));
                  error += fAbs(tmp-s[i][j]);
                  s[i][j] = tmp;
                }
              s[i][0] = (   beta[i][0]*(s[i+1][0]-s[i-1][0]) - s[i][2] + 4*s[i][1]     )/3;
          }




        }
      if(error < data->residuum){
          data->error =error;
          data->neededIter = curIter;
          return true;
      }
  }
  data->error =error;
  data->neededIter = curIter;
  return true;
}


bool gaussseidel(sData* data, double** s)
{
  int curIter=0;

  float error ;
  float tmp;

  while(curIter<data->maxIter) {
      std::cout << "\r\tGauss-Seidel: Iteration " << ++curIter;
      error = 0;
      for(int i = 1; i < data->dimI-1; i++)
        {
          for(int j = 1 ; j < data->dimJ-1; j++)
            {
              //Iterate over all values except border values
              tmp =(s[i-1][j]+s[i+1][j]+s[i][j+1]+s[i][j-1])/4.0;
              error += fAbs(tmp-s[i][j]);

              s[i][j] = tmp;

            }

        }


      if(error < data->residuum)
        return true;

  }
  return true;
}


bool jacobi(sData* data, double** s)
{
  int curIter=0;
  double** tmp_ptr;

  double** s_old = s;
  double** s_new = allocGrid1Mem(data,MAXDOUBLE);
  double error;
  double temp;

  for (int i= 0; i<data->dimI;i++){
      s_new[i][0] = s_old[i][0];
      s_new[i][data->dimJ-1]=s_old[i][data->dimJ-1];
  }
  for (int i=0;i<data->dimJ;i++){
      s_new[0][i] = s_old[0][i];
      s_new[data->dimI-1][i]=s_old[data->dimI-1][i];
  }

  while(curIter<data->maxIter ) {
      std::cout << "\r\tJakobi: Iteration " << ++curIter;
      error = 0;

      for (int i=1;i< data->dimI-1;i++){

          for (int j=1;j<data->dimJ-1;j++){
              temp = (s_old[i-1][j] +s_old[i+1][j] + s_old[i][j-1]+s_old[i][j+1])/4.0;
              error += fAbs( temp- s_old[i][j]);
              s_new[i][j] = temp;
          }
      }

      tmp_ptr = s_old;
      s_old = s_new;
      s_new  = tmp_ptr;

      if (error < data->residuum) return true;

  }

  // sync data-fields if nessessary
  if(s!=s_old) {
      for(int i=0; i<data->dimI; i++) {
          for(int j=0; j<data->dimJ; j++) {
              s_new[i][j] = s_old[i][j];
          }
      }
      tmp_ptr = s_old;
      s_old = s_new;
      s_new = tmp_ptr;
  }

  // free temp. memory (make sure you are not freeing memory s points to!)
  freeGrid1Mem(data,s_new);

  return true;
}

//------------------------------------------------------
