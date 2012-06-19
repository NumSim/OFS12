
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
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "output.h"
#include "data.h"

//------------------------------------------------------
bool output(sData* data)
{
        std::cout << "\nOutput:\n-------\n";

        if(!saveFaces(data)){ return false; }
        if(!saveCells(data)){ return false; }

        return true;
}

//------------------------------------------------------
bool saveFaces(const sData* data)
{
        const char* facesName="run/faces2.cfg";


        std::ofstream facesFile(facesName);
        if(!facesFile) { return false;  }
        facesFile.clear();

        // face coordinates
        facesFile << "faces\t" << (data->dimY* (data->dimX+1)) +((data->dimY+1)* data->dimX) << std::endl;
    facesFile << "#\tid \tx   \ty   \tdeltax   \tdeltay" << std::endl;
        // horizontal faces

    int counter = 0;
    double deltaX = data->sizeX/data->dimX;
    double deltaY = data->sizeY/data->dimY;
    for (int j=0;j<data->dimY+1;j++){
        for ( int i=0;i< data->dimX;i++){
            facesFile << "\t" << counter  << "\t"
                      << i*deltaX << "\t"
                      << j*deltaY << "\t"
                      << deltaX   << "\t"
                      << 0        << "\n" ;
            counter++;
        }
    }

        // vertical faces
    for ( int i=0;i< data->dimX+1;i++){
        for (int j=0;j<data->dimY;j++){
            facesFile << "\t" << counter  << "\t"
                      << i*deltaX << "\t"
                      << j*deltaY << "\t"
                      << 0        << "\t"
                      << deltaY   << "\n" ;
            counter++;
        }
    }
        // face boundaries
        facesFile << std::endl;
        facesFile << "boundaries\t"  << std::endl;
        facesFile << "# type:   1=SKIP FLUX CALC, 2=CONST.FLUX       junk: 9999.9" << std::endl;
        facesFile << "#\tid \tbType \tvalue0 \tvalue1" << std::endl;

        // skip flux calc on east,west boundary faces
    //int numberPoints = (data->dimY* (data->dimX+1)) +((data->dimY+1)* data->dimX);
    for (int j=0;j<data->dimY;j++){
    // upper & lower x boundaries
        facesFile << "\t" << j+data->dimX*data->dimY +data->dimX<< "\t 1 \t 9999.9 \t 9999.9 \n" ;
        facesFile << "\t" << j+2*data->dimX*data->dimY +data->dimX<< "\t 1 \t 9999.9 \t 9999.9 \n" ;
    }
    for (int i=0;i<data->dimX;i++){
    // upper & lower x boundaries
        facesFile << "\t" << i << "\t 2 \t 9999.9 \t 0 \n" ;
        facesFile << "\t" << i+data->dimX*data->dimY << "\t 2 \t 9999.9 \t 0 \n" ;
    }
        // const. flux calc on north,south boundary faces
            // TODO ? haven't we done that?


        facesFile.close();

        return true;
}

//------------------------------------------------------
bool saveCells(const sData* data)
{
        const char* cellsName="run/cells2.cfg";

        std::ofstream cellsFile(cellsName);
        if(!cellsFile) { return false;  }
        cellsFile.clear();

        // cell coordiantes
        cellsFile << "cells\t" << (data->dimY *data->dimX) << std::endl;
        cellsFile << "#\tcId \tcType \tx \ty \tface0 \tface1 \tface2 \tface3" << std::endl;
        int counter = 0;
        double deltaX = data->sizeX/data->dimX;
        double deltaY = data->sizeY/data->dimY;
        for (int j=0;j<data->dimY;j++){
            for(int i=0;i<data->dimX;i++){
                cellsFile << "\t" <<counter << "\t"
                    << 4 << "\t"
                    <<i*deltaX+deltaX/2 << "\t"
                    <<j*deltaY+deltaY/2 << "\t"
                    <<j*data->dimX+i << "\t"
                    << data->dimX*(data->dimY+1)+j+(i+1)*data->dimY << "\t"
                    <<(j+1)*data->dimX+i << "\t"
                    << data->dimX*(data->dimY+1)+j+i*data->dimY << "\n";



                counter++;
            }
        }

        // cell initvalues
        cellsFile << std::endl;
        cellsFile << "initvalues\t"  << std::endl;
        cellsFile << "#\tid \tphi" << std::endl;
        cellsFile << "\t-1      0.0     # default value" << std::endl;

        // cell boundaries
        cellsFile << std::endl;
        cellsFile << "boundaries\t"  << std::endl;
        cellsFile << "#\tcId \tbType \tvalue0" << std::endl;

        // TODO i have no idea what im doing here
        // this is just copy paste from original file
        // write left wall
     /*  for (int i=data->dimX*(data->dimY+1);i<data->dimX*(data->dimY+1)+data->dimY;i++){
         cellsFile <<"\t"<< i <<" \t 1 \t 30.0 \n";
       }
       for (int i=data->dimX*data->dimY+data->dimX*(data->dimY+1);i<data->dimX*data->dimY+data->dimX*(data->dimY+1)+data->dimY;i++){
           cellsFile <<"\t"<< i <<" \t 1 \t 10.0 \n";
         }

         the part commented out creates the same file as in faces.cfg but it is wrong? doenst make sense
         to have cells # > 50..
         */
        for (int i=0;i<data->dimY;i++){
            cellsFile <<"\t"<< i*data->dimX <<" \t 1 \t 30.0 \n";

        }
        for (int i=0;i<data->dimY;i++){
            cellsFile <<"\t"<< i*data->dimX+data->dimX-1 <<" \t 1 \t 10.0 \n";

        }



        cellsFile.close();

        return true;
}

