/***************************************************************************
 *   Copyright (C) 2010 by  Institute of Combustion Technology             *
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

#include "isogrid.h"
#include "isolines.h"
#include "data.h"

//------------------------------------------------------
bool isogrid(sData* data)
{
	std::cout << "\nOutput:\n-------\n";

	if(!saveCrossPoints(data)){ return false; }
	if(!saveIsoFaces(data)){ return false; }
	if(!saveIsoCells(data)){ return false; }

	return true;
}

//------------------------------------------------------
bool saveCrossPoints(const sData* data)
{
    const char* crossName="cross.txt";
	int id=0;

	std::ofstream crossFile(crossName);
	if(!crossFile) { return false;	}
	crossFile.clear();

	int nStrLines=10;
	int nStrCount=10;
	LINE *strLines=0;
	genISOLines(data, data->s1, strLines, nStrLines, nStrCount);

	int nPotLines=10;
	int nPotCount=10;
	LINE *potLines=0;
	genISOLines(data, data->s2, potLines, nPotLines, nPotCount);

    double x; double y;
    for(int i=0;i<nStrLines;i++) {
        for(int j=0;j<nPotLines;j++) {
            if(lineIntersection(strLines[i], potLines[j], x,y)) {
                crossFile << i << j << x << y << std::endl;
            }
        }
    }

	crossFile.close();

	//free(strLines);
	//free(potLines);
	return true;
}

//------------------------------------------------------
bool saveIsoFaces(const sData* data)
{
    const char* facesName="faces.cfg";
	int id=0;

	std::ofstream facesFile(facesName);
	if(!facesFile) { return false;	}
	facesFile.clear();

	// face coordinates
	facesFile << "faces\t" << (data->dimY* (data->dimX+1)) +((data->dimY+1)* data->dimX) << std::endl;
	facesFile << "#\tid \tx \ty \tdeltax \tdeltay" << std::endl;
		// TODO

	// face boundaries
	facesFile << std::endl;
	facesFile << "boundaries\t"  << std::endl;
	facesFile << "#	type:	1=SKIP FLUX CALC, 2=CONST.FLUX       junk: 9999.9" << std::endl;
	facesFile << "#\tid \tbType \tvalue0 \tvalue1" << std::endl;
		// TODO

	facesFile.close();

	return true;
}

//------------------------------------------------------
bool saveIsoCells(const sData* data)
{
	const char* cellsName="cells.cfg";
	int id=0;

	std::ofstream cellsFile(cellsName);
	if(!cellsFile) { return false;	}
	cellsFile.clear();

	// cell coordiantes
	cellsFile << "cells\t" << (data->dimY *data->dimX) << std::endl;
        cellsFile << "#\tcId \tcType \tx \ty \tface0 \tface1 \tface2 \tface3" << std::endl;
		// TODO

	// cell initvalues
	cellsFile << std::endl;
	cellsFile << "initvalues\t"  << std::endl;
	cellsFile << "#\tid \tphi" << std::endl;
	cellsFile << "\t-1	0.0	# default value" << std::endl;
		// TODO

	// cell boundaries
	cellsFile << std::endl;
	cellsFile << "boundaries\t"  << std::endl;
	cellsFile << "#\tcId \tbType \tvalue0" << std::endl;
		// TODO

	cellsFile.close();

	return true;
}
