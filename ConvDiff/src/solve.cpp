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

//------------------------------------------------------
bool solve(sData* data)
{
    sCell* curCell=0;
    sFace* curFace=0;

    std::cout << "\nCalculation:\n------------\n";

    // TODO

    std::cout << "\n";
    return true;
}

//------------------------------------------------------
void calcFlux(sData* data)
{
    static sFace* curFace=0;

    // numerical flux of each face
    for(int fId=0; fId<data->faceNo; fId++) {
        curFace=&data->faces[fId];

        // TODO
    }
}


