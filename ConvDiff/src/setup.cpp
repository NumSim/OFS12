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

#include "setup.h"
#include "data.h"
#include "output.h"
using namespace std;

//------------------------------------------------------
bool setup(sData* data)
{
	data->deltaT =data->maxTime/data->numberTimeSteps;
	cout << data->deltaT<< endl;
  sCell* curCell=0;
  sFace* curFace=0;
  data->deltaX =0.2;
  data->deltaY = 0.2; // TODO change this

  ////////////////////////
  // SET FACE VELOCITY  //
  ////////////////////////
  for(int fId=0; fId<data->faceNo; fId++) {
      curFace=&data->faces[fId];
      if (curFace->bType == 1 ){
      curFace->uv[0] = 0;//data->uv[0];
      curFace->uv[1] = 0;//data->uv[1];
      }else{
          curFace->uv[0] = 1;//0.4567;
          curFace->uv[1] = 0;//0.789;
      }
  }

  /////////////////////
  // SET CELL VOLUME //
  /////////////////////
  for(int cId=0; cId<data->cellNo; cId++) {
      curCell=&data->cells[cId];
      curCell->volume = ABS(curCell->cFaces[NORTH]->deltaxy[0]*
          curCell->cFaces[WEST]->deltaxy[1]);


  }

  /////////////////////////////
  // SET INITIAL CONDITIONS  //
  /////////////////////////////
  for(int cId=0; cId<data->cellNo; cId++) {
      curCell=&data->cells[cId];

     // curCell->phi[0] = SOMENUMBER;
     // curCell->phi[1] = ANOTHERNUMBER;
      curCell->p = 1;
  }

  //////////////////////////////
  // SET BOUNDARY CONDITIONS  //
  //////////////////////////////

  for(int fId=0; fId<data->faceNo; fId++) {
      curFace=&data->faces[fId];
      if (curFace->bType==2){
          curFace->numFlux[0] = curFace->bValue0;
          curFace->numFlux[1] = curFace->bValue1;

      }


  }
  for(int cId=0; cId<data->cellNo; cId++) {
      curCell=&data->cells[cId];
   //   std::cout<< "cId = " << cId << std::endl;
      if (curCell->bType==1){

          curCell->phi[0] = curCell->bValue0;
   //       std::cout<< "btype = 1 \n";
      }

  }

  return true;
}
