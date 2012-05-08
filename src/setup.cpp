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
#include "solve.h"
#include "transformation.h"

//------------------------------------------------------
bool setup(sData* data)
{
  std::cout << "\nSetup:\t------->\t";

  ///////////////////
  // SETUP U V     //
  ///////////////////
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->u[i][j] = 0;
          data->v[i][j] = 0;
      }
  }
  ///////////////////
  // SETUP Xi Eta  //
  ///////////////////
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->xi[i][j] = i*data->deltaXi;
          data->eta[i][j] = j* data->deltaEta;
      }
  }


  ///////////////////
  // SETUP X Y     //
  ///////////////////
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->x[i][j] = xof(data->xi[i][j],data->eta[i][j]);
          data->y[i][j] = yof(data->x[i][j],data->eta[i][j]);
      }
  }


  /////////////////////////////////
  // SETUP INITIAL SCALAR VALUES //
  /////////////////////////////////
  for(int i=0; i<data->dimI; i++) {
      for(int j=0; j<data->dimJ; j++) {
          data->phi[i][j] = 1.337;
          data->psi[i][j] = 1.337;
      }
  }


  //////////////////////////////////
  // SETUP BOUNDARY SCALAR VALUES //
  //////////////////////////////////
  // PHI
  for(int i=0; i<data->dimI; i++) {
      data->phi[i][0] =0;
      data->phi[i][data->dimJ-1]=0;
  }
  for(int j=0; j<data->dimJ; j++) {
      data->phi[0][j] = 2;
      data->phi[data->dimI-1][j]=0;
  }
  // PSI
  for(int i=0; i<data->dimI; i++) {
      data->psi[i][0] =0;
      data->psi[i][data->dimJ-1]=2;
  }
  for(int j=0; j<data->dimJ; j++) {
      data->psi[0][j] = 2.0*j/(data->dimJ-1);
      data->psi[data->dimI-1][j]=2.0*j/(data->dimJ-1);
  }

  std::cout << "Success!\n";
  return true;
}



