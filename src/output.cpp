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
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "output.h"
#include "data.h"

//------------------------------------------------------
bool output(sData* data)
{
  std::cout << "\nOutput below:";

  showScalar(data, "phi.dat", data->s1);

  if( !saveGrid(data, "output/grid") ) { return false; }
  if( !saveScalar(data, "output/phi.dat",data->s1) ){ return false; }
  if( !saveScalar(data, "output/u.dat",data->u) ){ return false; }
  if( !saveScalar(data, "output/v.dat",data->v) ){ return false; }

  std::cout << "Residual r = " << data->error << " reached, after " << data->neededIter << " iterations. \n";


  return true;
}

//------------------------------------------------------
void showScalar(const sData* data, const char* scalarName, double** s)
{
  const int maxHoriz=5;
  const int maxVert =5;

  std::cout.precision( 1 );

  std::cout << "\nY\t------------------------- " << scalarName << " -------------------------\n"
      << "^\n"
      << "|\n";

  double iStep,jStep;
  if(data->dimI<maxVert)  { iStep=1; } else { iStep=data->dimI/(double)maxVert; }
  if(data->dimJ<maxHoriz) { jStep=1; } else { jStep=data->dimJ/(double)maxHoriz;}

  double i,j=data->dimJ-1 + jStep;
  while(j>0) {
      j-=jStep; if(j<1){ j=0; }
      std::cout << std::fixed << (int)j << "\t";

      i=-iStep;
      while(i<data->dimI-1) {
          i+=iStep; if(i>data->dimI-2){ i=data->dimI-1; }
          std::cout.setf(std::ios::showpos);
          std::cout << std::scientific << s[(int)i][(int)j] << "  ";
          std::cout.unsetf(std::ios::showpos);
      }
      std::cout << "\n|\n";
  }
  std::cout << " --\t";

  i=-iStep;
  while(i<data->dimI-1) {
      i+=iStep; if(i>data->dimI-2){ i=data->dimI-1; }
      std::cout << "   -" << (int)i << "-    ";
  }
  std::cout << "->X\n\n";
}

//------------------------------------------------------
bool saveScalar(const sData* data, const char* fileName, double** s)
{
  // save node-solutions
  std::ofstream scalarFile(fileName);
  if(!scalarFile) { return false;	}
  scalarFile.clear();
  for(int i=0; i<data->dimI; i++){
      for(int j=0; j<data->dimJ; j++){
          scalarFile << " "<< s[i][j];
      }
      scalarFile << std::endl;
  }
  scalarFile.close();

  return true;
}

//------------------------------------------------------
bool saveGrid(const sData* data, const char* gridName)
{
  char fileNameX[80], fileNameY[80];

  // save node-positions
  sprintf(fileNameX,"%s.x.dat",gridName);
  sprintf(fileNameY,"%s.y.dat",gridName);
  std::ofstream meshXFile(fileNameX);
  std::ofstream meshYFile(fileNameY);
  if(!meshXFile) { return false;	}
  if(!meshYFile) { return false;	}
  meshXFile.clear();
  meshYFile.clear();
  for(int i=0; i<data->dimI; i++){
      for(int j=0; j<data->dimJ; j++){
          meshXFile << data->x[i][j] << " ";
          meshYFile << data->y[i][j] << " ";
      }
      meshXFile << std::endl;
      meshYFile << std::endl;
  }
  meshXFile.close();
  meshYFile.close();
  return true;
}
