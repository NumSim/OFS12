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

//------------------------------------------------------
bool setup(sData* data)
{
    sCell* curCell=0;
    sFace* curFace=0;

    ////////////////////////
    // SET FACE VELOCITY  //
    ////////////////////////
    for(int fId=0; fId<data->faceNo; fId++) {
        curFace=&data->faces[fId];

        // TODO
    }

    /////////////////////
    // SET CELL VOLUME //
    /////////////////////
    for(int cId=0; cId<data->cellNo; cId++) {
        curCell=&data->cells[cId];

        // TODO
    }

    /////////////////////////////
    // SET INITIAL CONDITIONS  //
    /////////////////////////////
    for(int cId=0; cId<data->cellNo; cId++) {
        curCell=&data->cells[cId];

        // TODO
    }

    //////////////////////////////
    // SET BOUNDARY CONDITIONS  //
    //////////////////////////////
    for(int fId=0; fId<data->faceNo; fId++) {
        curFace=&data->faces[fId];

        // TODO
    }
    for(int cId=0; cId<data->cellNo; cId++) {
        curCell=&data->cells[cId];

        // TODO
    }

    return true;
}
