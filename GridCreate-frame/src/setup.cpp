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
#include <math.h>

#include "setup.h"
#include "data.h"
#include "output.h"

//------------------------------------------------------
bool setup(sData* data)
{
	std::cout << "\nSetup:\n-------\n";

	// calc grid and save in data->x, data->y
	double cX=data->sizeX/(double)data->dimX;
	double cY=data->sizeY/(double)data->dimY;
	for(int x=0; x<data->dimX; x++) {
		for(int y=0; y<data->dimY; y++) {
			data->x[x][y]=x*cX +data->sizeX*0.5;
			data->y[x][y]=y*cY +data->sizeY*0.5;
		}
	}

	return true;
}
