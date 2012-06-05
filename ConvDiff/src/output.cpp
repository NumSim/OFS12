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
#include <fstream>

#include <stdio.h>
#include <math.h>

#include "output.h"
#include "data.h"

//------------------------------------------------------
bool output(sData* data)
{
    if(!saveData(data, "output/data")) return false;
   // showScalar(data, "output/data.phi", data->phi[0]);
    return true;
}

//------------------------------------------------------
bool saveData(const sData* data, const char* fileName)
{
    sCell *rowCell, *columnCell;
    double x[2],y[2];
    int east,west;
    char fileNameX[80], fileNameY[80],fileNamePhi[80];

    // open/create files
    sprintf(fileNameX,"%s.meshX",fileName);
    sprintf(fileNameY,"%s.meshY",fileName);
    sprintf(fileNamePhi,"%s.phi",fileName);
    std::ofstream meshXFile(fileNameX);
    std::ofstream meshYFile(fileNameY);
    std::ofstream phiFile(fileNamePhi);
    if(!meshXFile) { return false; }
    if(!meshYFile) { return false; }
    if(!phiFile)   { return false; }
    meshXFile.clear();
    meshYFile.clear();
    phiFile.clear();

    // search for the left-top-cell
    rowCell=&data->cells[0];
    while(rowCell->nCells[WEST]) { rowCell=rowCell->nCells[WEST]; };
    while(rowCell->nCells[NORTH]){ rowCell=rowCell->nCells[NORTH];};

    // save north row first
    columnCell=rowCell;
    // save east-south-coords
    x[0] = columnCell->cFaces[NORTH]->xy[0];
    x[1] = columnCell->cFaces[NORTH]->xy[0] +columnCell->cFaces[NORTH]->deltaxy[0];
    y[0] = columnCell->cFaces[NORTH]->xy[1];
    y[1] = columnCell->cFaces[NORTH]->xy[1] +columnCell->cFaces[NORTH]->deltaxy[1];
    if(x[0]<x[1]){ west=0; }
    else         { west=1; }
    meshXFile << x[west] << " ";
    meshYFile << y[west] << " ";

    while(columnCell){
        // save east-north-coords
        x[0] = columnCell->cFaces[NORTH]->xy[0];
        x[1] = columnCell->cFaces[NORTH]->xy[0] +columnCell->cFaces[NORTH]->deltaxy[0];
        y[0] = columnCell->cFaces[NORTH]->xy[1];
        y[1] = columnCell->cFaces[NORTH]->xy[1] +columnCell->cFaces[NORTH]->deltaxy[1];
        if(x[0]>x[1]){ east=0; }
        else         { east=1; }
        meshXFile << x[east] << " ";
        meshYFile << y[east] << " ";
        columnCell=columnCell->nCells[EAST];
    }
    meshXFile << std::endl;
    meshYFile << std::endl;

    // save all south rows
    while(rowCell){
        columnCell=rowCell;

        // save west-south-coords
        x[0] = columnCell->cFaces[SOUTH]->xy[0];
        x[1] = columnCell->cFaces[SOUTH]->xy[0] +columnCell->cFaces[SOUTH]->deltaxy[0];
        y[0] = columnCell->cFaces[SOUTH]->xy[1];
        y[1] = columnCell->cFaces[SOUTH]->xy[1] +columnCell->cFaces[SOUTH]->deltaxy[1];
        if(x[0]<x[1]){ west=0; }
        else         { west=1; }
        meshXFile << x[west] << " ";
        meshYFile << y[west] << " ";

        while(columnCell){
            // save east-south-coords
            x[0] = columnCell->cFaces[SOUTH]->xy[0];
            x[1] = columnCell->cFaces[SOUTH]->xy[0] +columnCell->cFaces[SOUTH]->deltaxy[0];
            y[0] = columnCell->cFaces[SOUTH]->xy[1];
            y[1] = columnCell->cFaces[SOUTH]->xy[1] +columnCell->cFaces[SOUTH]->deltaxy[1];
            if(x[0]>x[1]){ east=0; }
            else         { east=1; }
            meshXFile << x[east] << " ";
            meshYFile << y[east] << " ";
            phiFile   << columnCell->phi[0] << " ";
            columnCell=columnCell->nCells[EAST];
        }
        rowCell=rowCell->nCells[SOUTH];

        meshXFile << std::endl;
        meshYFile << std::endl;
        phiFile   << std::endl;
    }
    meshXFile.close();
    meshYFile.close();

    return true;
}
//------------------------------------------------------
/*
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
}*/
