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
#include <omp.h>

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
  std::cout << "\nSolve:\t------->\t \n";

  if(!gaussseidel(data,data->s1)){ return false; }
  std::cout << "Success!\n";
  return true;
}

//------------------------------------------------------


bool gaussseidel(sData* data, double** s)
{

  int N,M;
  N=data->dimI;
  M=data->dimJ;
  double dxi1, dxi2, deta1,deta2,fdx;
  dxi1 = data->deltaXi;
  deta1 = data->deltaEta;
  fdx = data->finiteDiffDx;
  dxi2 = dxi1*dxi1;
  deta2= deta1*deta1;

  // allocate memory for derivatives & coeff
  double ***alpha = new double**[N];
  double **beta = new double*[N];
  double **dxiDx = new double*[N];
  double **dxiDy = new double*[N];
  double **detaDx = new double*[N];
  double **detaDy = new double*[N];
  double **ddxiDDx = new double*[N];
  double **ddxiDDy = new double*[N];
  double **ddetaDDx = new double*[N];
  double **ddetaDDy = new double*[N];
  double **dhDx = new double*[N];

  for (int i=0;i<N;i++){
      dxiDx[i] = new double[M];
      dxiDy[i] = new double[M];
      detaDx[i] = new double[M];
      detaDy[i] = new double[M];
      ddxiDDx[i] = new double[M];
      ddxiDDy[i] = new double[M];
      ddetaDDx[i] = new double[M];
      ddetaDDy[i] = new double[M];
      alpha[i] = new double* [M];
      beta[i] = new double[2];
      dhDx[i] = new double[2];
  }

  for (int i=0;i<N;i++){
      for(int j=0;j<M;j++){
          alpha[i][j] = new double[5];
      }
  }

  // Calculate derivatives
  dxi(data,dxiDx,dxiDy);
  deta(data,detaDx,detaDy);
  ddxi(data,ddxiDDx,ddxiDDy);
  ddeta(data,ddetaDDx,ddetaDDy);
  for (int i=0;i<N;i++){
      dhDx[i][0] = (ybottomof(data->x[i][0]+fdx,data->y[i][0]) -ybottomof(data->x[i][0]-fdx,data->y[i][0])      )/(2*fdx);
      dhDx[i][1] = (ytopof(data->x[i][M-1]+fdx,data->y[i][M-1]) -ytopof(data->x[i][M-1]-fdx,data->y[i][M-1]))/(2*fdx);
  }


  // Calculate alpha for inside
  for (int i=1;i<N-1;i++){
      for(int j=1;j<M-1;j++){
          alpha[i][j][0] = (dxiDx[i][j]*dxiDx[i][j]+dxiDy[i][j]*dxiDy[i][j] )/dxi2;
          alpha[i][j][1] = (detaDx[i][j]*detaDx[i][j]+detaDy[i][j]*detaDy[i][j] )/deta2;
          alpha[i][j][2] = 2.*(dxiDx[i][j]*detaDx[i][j]+dxiDy[i][j]*detaDy[i][j])/deta1/dxi1;
          alpha[i][j][3] = (ddxiDDx[i][j]+ddxiDDy[i][j])/dxi1;
          alpha[i][j][4] = (ddetaDDx[i][j]+ddetaDDy[i][j])/deta1;
      }
  }

  // Calculate beta for wall
  for (int i=1;i<N-1;i++){
      beta[i][0] = (dhDx[i][0]*dxiDx[i][0]-dxiDy[i][0])/(dhDx[i][0]*detaDx[i][0]-detaDy[i][0])* deta1/dxi1;
      beta[i][1] = (dhDx[i][1]*dxiDx[i][M-1]-dxiDy[i][M-1])/(dhDx[i][1]*detaDx[i][M-1]-detaDy[i][M-1])* deta1/dxi1;
  }

  // free memory
  for (int i=0;i<N;i++){
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

  // Iterate over spatial domain
  int curIter=0;
  double a1,a2,a3,a4,a5, error, tmp;
  error = 1337;

  while(curIter<data->maxIter) {


      ++curIter;
      if (curIter%1000==0 ){
          std::cout << "\tGauss-Seidel: Iteration " << curIter << ", Residual= " << error << std::endl;
          data->errorLog.push_back(error);
      }
      error =0;

      int i,tid, nthreads;
      #pragma omp parallel shared(s) private(i,tid) reduction (+:error)
      {
        tid = omp_get_thread_num();
        if (tid == 0)
          {
          nthreads = omp_get_num_threads();
          }
    //    std::cout << 1+(tid*data->dimI-1)/2 << " <-from , to->" << (data->dimI-1)/2+ tid * (data->dimI)/2 << std::endl;
     //   #pragma omp for
    //    for( i = 1+(tid*(data->dimI-1))/2; i < (data->dimI-1)/2+ tid * (data->dimI)/2; i++)

        for (int i=1;i<data->dimI-1;i++)
          {



            s[i][0] = (   beta[i][0]*(s[i+1][0]-s[i-1][0]) - s[i][2] + 4*s[i][1]     )/3;

            for(int j = 1 ; j < data->dimJ-1; j++)
              {
                a1 = alpha[i][j][0];
                a2 = alpha[i][j][1];
                a3 = alpha[i][j][2];
                a4 = alpha[i][j][3];
                a5 = alpha[i][j][4];

                tmp = s[i+1][j+1]   * (a3/4.0)
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
      }
      if(error < data->residuum){
          std::cout << "\tFinal Gauss-Seidel: Iteration " << curIter << ", Residual= " << error;
          data->error =error;
          data->neededIter = curIter;
          return true;
      }
  }
  std::cout << "\tFinal Gauss-Seidel: Iteration " << curIter << ", Residual= " << error;
  data->error =error;
  data->neededIter = curIter;
  return true;
}


bool postprocessing(sData* data)
{
  std::cout << "Postprocessing:\t------->\t";



  int N,M;
  N=data->dimI;
  M=data->dimJ;
  double **dxiDx = new double*[N];
  double **dxiDy = new double*[N];
  double **detaDx = new double*[N];
  double **detaDy = new double*[N];
  double dphiDxi;
  double dphiDeta;

  for (int i=0;i<N;i++){
      dxiDx[i] = new double[M];
      dxiDy[i] = new double[M];
      detaDx[i] = new double[M];
      detaDy[i] = new double[M];

  }
  // Calculate derivatives
  dxi(data,dxiDx,dxiDy);
  deta(data,detaDx,detaDy);
  for (int i=1;i<N-1;i++){
      for (int j=1;j<M-1;j++)
        {
          dphiDxi = (data->s1[i+1][j]-data->s1[i-1][j])/(2*data->deltaXi);
          dphiDeta = (data->s1[i][j+1]-data->s1[i][j-1])/(2*data->deltaEta);
          data->u[i][j] = dphiDxi*dxiDx[i][j] + dphiDeta*detaDx[i][j];
          data->v[i][j] = dphiDxi*dxiDy[i][j] + dphiDeta*detaDy[i][j];


        }
  }

  std::cout << "Success!\n";
  return true;
}
