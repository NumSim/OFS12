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
#include <iostream>

#include <stdio.h>
#include <math.h>

#include "solve.h"
#include "data.h"
using namespace std;
//------------------------------------------------------
bool
solve(sData* data)
{
  sCell* curCell = 0;
  sFace* curFace = 0;

  cout << "\nCalculation:\n------------\n";

  double dphidt;
  double dt = data->maxTime / data->numberTimeSteps;

  for (int t = 0; t < data->numberTimeSteps; t++)
    {
      calcFlux(data);
      if (t % 10000 == 0 )
        cout << "i= " << t << endl;
      for (int i = 0; i < data->cellNo; i++)
        {

          curCell = &data->cells[i];
          if (curCell->bType == 1)
            {
              // skip
            }
          else if (curCell->bType == 2)
            {

            }
          else
            {

              //WARNING, FLUX DIRECTION ? always left to right?
              //    cout << " delta xy = "<< curCell->cFaces[WEST]->deltaxy[1]<<endl;
              //    cout << " delta xy = "<< curCell->cFaces[NORTH]->deltaxy[0]<<endl;
              //   cout << "factor 1 " << (curCell->cFaces[WEST]->numFlux[0]-
              //      curCell->cFaces[EAST]->numFlux[0])<< endl;
              // cout << " summand 1 = "<< curCell->cFaces[WEST]->numFlux[0]<< endl;
              //  cout << " summand 2 = "<< curCell->cFaces[EAST]->numFlux[0]<< endl;
              //    cout << "factor 1 " << (curCell->cFaces[NORTH]->numFlux[1]-
              //      curCell->cFaces[SOUTH]->numFlux[1])<< endl;

              dphidt = (curCell->cFaces[WEST]->numFlux[0]
                  - curCell->cFaces[EAST]->numFlux[0])
                  * ABS(curCell->cFaces[WEST]->deltaxy[1])
                  + (-curCell->cFaces[NORTH]->numFlux[1]
                      + curCell->cFaces[SOUTH]->numFlux[1])
                      * ABS(curCell->cFaces[NORTH]->deltaxy[0]);

              dphidt /= (curCell->volume * data->rho);
              //
           //   std::cout << dphidt << "\n";
              //    cout << " dphidt = " << dphidt << endl;

              curCell->phi[0] = curCell->phi[0] + dphidt * dt;

            }

        }
    }

  std::cout << "\n";
  return true;
}

//------------------------------------------------------
void
calcFlux(sData* data)
{
  //std::cout << "in calcFlux\n";
  static sFace* curFace = 0;
  double vel;

  // numerical flux of each face
  for (int fId = 0; fId < data->faceNo; fId++)
    {
     // std::cout << fId << "\n";
      curFace = &data->faces[fId];
      // check whether or not to update
      if (curFace->bType == 1)
        {
          // if bType == 1 SKIP FLUX CALC
         // std::cout << fId << "type 1" << std::endl;
        }
      else if (curFace->bType == 2)
        {
        //  std::cout << fId << "type 2 " << std::endl;
          // if bType ==2 CONST FLUX
          //curFace->numFlux[1] = 0; // ?
          //curFace->numFlux[0] = 0;
        }
      else
        {
          //     cout << " --------------------" << endl;
          // check orientation
          //      cout << curFace->nCells[0]->phi[0] << " "<< curFace->nCells[1]->phi[0] << endl;

          if (curFace->deltaxy[0] == 0)
            {
              //    |
           //   std::cout << "| \n";

              vel = (curFace->uv[0] <0)?curFace->nCells[1]->phi[0]:curFace->nCells[0]->phi[0];

              curFace->numFlux[1] = 0;
              curFace->numFlux[0] =
                  data->rho * curFace->uv[0] *
                  // (curFace->nCells[0]->phi[0]+curFace->nCells[1]->phi[0])/2
                      vel
                      - data->alpha
                          * (curFace->nCells[1]->phi[0]
                              - curFace->nCells[0]->phi[0])
                          / (curFace->nCells[1]->xy[0]
                              - curFace->nCells[0]->xy[0]);
              //       cout << " | " << endl;
             //        cout << (curFace->nCells[1]->phi[0]- curFace->nCells[0]->phi[0]) << "<- is this zero \n";
             //  cout << " flux = " << curFace->numFlux[0] << endl;

            }
          else
            {
              //    -
             // std::cout << "| \n";
              vel = (curFace->uv[0] <0)?curFace->nCells[1]->phi[0]:curFace->nCells[0]->phi[0];

              curFace->numFlux[0] = 0;
              curFace->numFlux[1] =
                  data->rho * curFace->uv[1] *
                  //(curFace->nCells[0]->phi[0]+curFace->nCells[1]->phi[0])/2
                      vel
                      - data->alpha
                          * (curFace->nCells[1]->phi[0]
                              - curFace->nCells[0]->phi[0])
                          / (curFace->nCells[1]->xy[1]
                              - curFace->nCells[0]->xy[1]);
               //    cout << " -" << endl;
               //    cout << (curFace->nCells[1]->phi[0]- curFace->nCells[0]->phi[0]) << "<- is this zero \n";
             //   cout << " flux = " << curFace->numFlux[0] << endl;

            }

        }
      //curFace = &data->faces[16];
      //std::cout << curFace->numFlux[0] << " " << curFace->numFlux[1] << std::endl;
    }
}

