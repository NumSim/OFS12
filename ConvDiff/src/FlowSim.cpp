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

#include "data.h"
#include "input.h"
#include "setup.h"
//#include "solve.h"
#include "output.h"
#include "simple.h"

//------------------------------------------------------
int main(int, char**)
{
    sData* data = new sData;
    const char* cfgFilePath  = "/home/tc497585/programming/workspace/OFS12/ConvDiff/run/data.cfg";
    const char* faceFilePath = "/home/tc497585/programming/workspace/OFS12/GridCreate-frame/run/faces2.cfg";
    const char* cellFilePath = "/home/tc497585/programming/workspace/OFS12/GridCreate-frame/run/cells2.cfg";

    std::cout << "\t Simulationstechnik V\n"
            << "\t====== FlowSim ======\n";

    // read config from input file
    if(!input(cfgFilePath, faceFilePath, cellFilePath, data)) {
        std::cout << "ERROR while reading input file...exiting";
        getchar();
        return 1;
    }

    // setup data (boundaries, initial data, etc.)
    if(!setup(data)) {
        std::cout << "ERROR while data setup...exiting";
        getchar();
        return 1;
    }

    // iterativ solver
   std::cout << "simple"<<std::endl;
    if(!simple(data)) {
        std::cout << "ERROR while solving...exiting";
        getchar();
        return 1;
    }

    // output data
    if(!output(data)) {
        std::cout << "ERROR while data output...exiting";
        getchar();
        return 1;
    }

    std::cout << "Success...";
    getchar();

    delete data;
    return 0;
}
