/***************************************************************************
 *   Copyright (C) 2006 by  Institute of Combustion Technology             *
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
#include <stdlib.h>

#include "data.h"

//------------------------------------------------------
double** allocGrid1Mem(const sData* const data, const double preset)
{
  // allocate memory -- no error-check
  double** tmpPtr = (double**)malloc(data->dimI*sizeof(double*));
  for(int x=0; x<data->dimI; x++) {
      tmpPtr[x] = (double*)malloc(data->dimJ*sizeof(double));
      for(int y=0; y<data->dimJ; y++) { tmpPtr[x][y] = preset; }
  }
  return tmpPtr;
}

//------------------------------------------------------
void freeGrid1Mem(const sData* const data, double** mem)
{
  for(int i=0; i<data->dimI; i++) {
      free(mem[i]);
  }
}

//------------------------------------------------------
double*** allocGridXMem(const sData* const data, const int vSize, const double preset)
{
  // allocate memory -- no error-check
  double*** tmpPtr = (double***)malloc(vSize*sizeof(double**));
  for(int s=0; s<vSize; s++) {
      tmpPtr[s] = (double**)malloc(data->dimI*sizeof(double*));
      for(int i=0; i<data->dimI; i++) {
          tmpPtr[s][i] = (double*)malloc(data->dimJ*sizeof(double));
          for(int j=0; j<data->dimJ; j++) { tmpPtr[s][i][j] = preset; }
      }
  }
  return tmpPtr;
}
