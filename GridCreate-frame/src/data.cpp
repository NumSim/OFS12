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
#include <stdlib.h>
#include <iostream>

#include "data.h"

//------------------------------------------------------
double** allocGrid1Mem(const sData* const data, const double preset)
{
	// allocate memory -- no error-check
	double** tmpPtr = (double**)malloc(data->dimX*sizeof(double*));
	for(int x=0; x<data->dimX; x++) {
		tmpPtr[x] = (double*)malloc(data->dimY*sizeof(double));
		for(int y=0; y<data->dimY; y++) { tmpPtr[x][y] = preset; }
	}
	return tmpPtr;
}
