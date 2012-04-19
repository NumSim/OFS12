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
  //if(!jacobi(data, data->s1))	{ return false; }
  //if(!thomas(data,data->s1))r	{ return false; }
  std::cout << "Success!\n";
  return true;
}

//------------------------------------------------------


bool gaussseidelMorphed(sData* data, double** s)
{
  int curIter=0;
  double error;
  float tmp;
  double a1,a2,a3,a4,a5;
  int N=data->dimI-2;
  int M=data->dimJ-2;

  // allocate memory for derivatives
  double ***alpha = new double**[N+2];
  double **temp1 = new double*[N+2];
  double **temp2 = new double*[N+2];
  double **temp3 = new double*[N+2];
  double **temp4 = new double*[N+2];
  double **temp5 = new double*[N+2];
  double **temp6 = new double*[N+2];
  double **temp7 = new double*[N+2];
  double **temp8 = new double*[N+2];

  for (int i=0;i<N+2;i++){
      temp1[i] = new double[M+2];
      temp2[i] = new double[M+2];
      temp3[i] = new double[M+2];
      temp4[i] = new double[M+2];
      temp5[i] = new double[M+2];
      temp6[i] = new double[M+2];
      temp7[i] = new double[M+2];
      temp8[i] = new double[M+2];
      alpha[i] = new double* [M+2];
  }
  for (int i=0;i<N+2;i++){
      for(int j=0;j<M+2;j++){
          alpha[i][j] = new double[5];
      }
  }

  // write derivatives
  dxi(data,temp1,temp2);
  deta(data,temp3,temp4);
  ddxi(data,temp5,temp6);
  ddeta(data,temp7,temp8);

  // calculate alpha
  for (int i=1;i<data->dimI-1;i++){
      for(int j=1;j<data->dimJ-1;j++){
          alpha[i][j][0] = temp1[i][j]*temp1[i][j]+temp2[i][j]*temp2[i][j];     //alpha1
          alpha[i][j][1] = temp3[i][j]*temp3[i][j]+temp4[i][j]*temp4[i][j];     //alpha2
          alpha[i][j][2] = 2*(temp1[i][j]*temp3[i][j]+temp2[i][j]*temp4[i][j]); //alpha3
          alpha[i][j][3] = temp5[i][j]+temp6[i][j];                             //alpha4
          alpha[i][j][4] = temp7[i][j]+temp8[i][j];                             //alpha5
      }
  }

  // free memory
  for (int i=0;i<N+2;i++){
      delete[] temp1[i];
      delete[] temp2[i];
      delete[] temp3[i];
      delete[] temp4[i];
      delete[] temp5[i];
      delete[] temp6[i];
      delete[] temp7[i];
      delete[] temp8[i];
  }
  delete[] temp1;
  delete[] temp2;
  delete[] temp3;
  delete[] temp4;
  delete[] temp5;
  delete[] temp6;
  delete[] temp7;
  delete[] temp8;

  while(curIter<data->maxIter) {
      /*std::cout << "\r\tGauss-Seidel: Iteration " << */++curIter;
      error =0;
      for(int i = 1; i < data->dimI-1; i++)
        {
          for(int j = 1 ; j < data->dimJ-1; j++)
            {
              a1 = alpha[i][j][0];
              a2 = alpha[i][j][1];
              a3 = alpha[i][j][2];
              a4 = alpha[i][j][3];
              a5 = alpha[i][j][4];

              tmp =    s[i+1][j+1]   * (a3/4.f)
                     + s[i+1][j]     * (a1+a4/2.f)
                     + s[i+1][j-1]   * (-a3/4.f)
                     + s[i][j+1]     * (a2+a5/2.f)
                     + s[i][j-1]     * (a2-a5/2.f)
                     + s[i-1][j+1]   * (-a3/4.f)
                     + s[i-1][j]     * (a1-a4/2.f)
                     + s[i-1][j-1]   * (a3/4.f);
              tmp /=(2*(a1+a2));

              error += fAbs(tmp-s[i][j]);
              s[i][j] = tmp;
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



