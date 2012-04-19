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
#include "setup.h"
#include "data.h"
#include "transformation.h"

//------------------------------------------------------
bool setup(sData* data)
{
  std::cout << "\nSetup:\t------->\t";

  ///////////////////
  // SETUP Xi Eta-GRID //
  ///////////////////
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->xi[i][j] = i*data->deltaXi;
          data->eta[i][j] = j* data->deltaEta;
      }
  }
  ///////////////////
  // SETUP X Y-GRID //
  ///////////////////
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->x[i][j] = xof(data->xi[i][j],data->eta[i][j]);
          data->y[i][j] = yof(data->xi[i][j],data->eta[i][j]);
      }
  }

  /////////////////////////////////
  // SETUP INITIAL SCALAR VALUES //
  /////////////////////////////////
  // set whole field 's1' to start value
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->s1[i][j] = 13.37;
      }
  }

  //////////////////////////////////
  // SETUP BOUNDARY SCALAR VALUES //
  //////////////////////////////////

  /* MORE COMPLEX CASE !! DONT USE !!*/
 /* float Eover2PI = 1;
  float x =0;
  float y = 0; */
  float u_inf =0;
  float v_inf = 1;



  for(int i=0; i<data->dimI; i++) {
      //data->s1[i][0] = 1;
      //data->s1[i][data->dimJ-1] = 1;
      /* MORE COMPLEX CASE !! DONT USE !!*/
      //x = data->x[i][0] - 1.1f;
      //y = 0 - 0.5f;
      data->s1[i][0] = u_inf*data->x[i][0]+v_inf*data->y[i][0];//u_inf*data->x[i][0] + Eover2PI * log((x*x+y*y))/2;
      //x = data->x[i][data->dimJ-1]- 1.1f;
      data->s1[i][data->dimJ-1]= u_inf*data->x[i][data->dimJ-1] +v_inf*data->y[i][data->dimJ-1];//+ Eover2PI * log((x*x+y*y))/2;

  }
  for(int j=0; j<data->dimJ; j++) {
      //data->s1[0][j] = 1;
      //data->s1[data->dimI-1][j] = 1;
      /* MORE COMPLEX CASE !! DONT USE !!*/
      //x = 0- 1.1f;
      //y = data->y[0][j] - 0.5f;
      data->s1[0][j] = u_inf*data->x[0][j]+v_inf*data->y[0][j]; //u_inf*0 + Eover2PI * log((x*x+y*y))/2;
      //x = 0-1.1f;
      //y = data->y[data->dimI-1][j]- 0.5f;
      data->s1[data->dimI-1][j]= u_inf*data->x[data->dimI-1][j]+v_inf*data->y[data->dimI-1][j];//u_inf*1.f +Eover2PI * log((pow(0.1,2)+y*y))/2;

  }
  std::cout << "Success!\n";
  return true;
}



