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
#ifndef DATA_H
#define DATA_H

#include <limits>

#define PETER	3400
#define THOMAS 6706

#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#define ABS(x)   ( ((x)> 0)  ? (x):-(x))

#define DECIDE(a,b,c) ( ((a)==(c)) ? (a):(b) )

#define MAXDOUBLE	(std::numeric_limits<double>::max())
#define MINDOUBLE	(std::numeric_limits<double>::min())
#define MAXINT	    (std::numeric_limits<int>::max())
#define MININT	    (std::numeric_limits<int>::min())

#define SOUTH 0
#define EAST  1
#define NORTH 2
#define WEST  3

#define NEIGHB_SW	0
#define NEIGHB_NE	1

// boundary types
#define WALL     1
#define PERIODIC 2

// cell types
#define PIXEL	4

struct sFace;
struct sCell;
struct sData;

//------------------------------------------------------
struct sFace {
        sFace() : id(-1), bType(0) {}

        int     id;

	// grid settings

	double	xy[2];			// x,y-coordinate
	double	deltaxy[2];		// dx,dy relative to x,y
	sCell*	nCells[2];		// two neighbour cells

	// boundary settings
        int	bType;			// boundary type
	double	bValue0;		// boundary value 0
	double	bValue1;		// boundary value 1

	// numerical settings
	double	numFlux[2];		// numerical flux in x,y

	// physical settings
	double	uv[2];			// velocity
};

//------------------------------------------------------
struct sCell {
        sCell() : id(-1), cType(0), bType(0) {}

        int     id;

	// grid settings
	int		cType;			// cell type
	double	xy[2];			// cell center
	int		noFaces;		// number of faces
	sFace** cFaces;			// cell faces
	sCell** nCells;			// neighbour cells

	// boundary settings
	int		bType;			// boundary type
	double	bValue0;		// boundary value

	// physical settings

	// numerical settings
	double	volume;			// cell volume
	double	fluxBalance;	// flux balance of cell
	double	phi[2];			// phi at cell center
};

//------------------------------------------------------
struct sData {
	// cells
	int		cellNo;
	sCell*	cells;
	double	initPhi;

	// faces
	int		faceNo;
	sFace*	faces;

	// numerical settings
	double	maxTime;
	int		numberTimeSteps;

	double	overrelax;
	double	residuum;

	// physical settings
	double	alpha;			// diffusion coefficient [lenght^2/time]
	double	rho;			// density
	double	uv[2];			// velocity
};

#endif
