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
#ifndef DATA_H
#define DATA_H

#include <limits>

#define WITHOUT_INFLUENCE	4711.0

#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#define ABS(x)   ( ((x)> 0)  ? (x):-(x))

#define DECIDE(a,b,c) ( ((a)==(c)) ? (a):(b) )

#define MAXDOUBLE   (std::numeric_limits<double>::max())
#define MINDOUBLE   (std::numeric_limits<double>::min())
#define MAXINT	    (std::numeric_limits<int>::max())
#define MININT	    (std::numeric_limits<int>::min())

// used for faces of pixelCells->cFaces[0..3] and pixelCells->nCells[0..3]
enum senwDirection {
    SOUTH = 0,
    EAST  = 1,
    NORTH = 2,
    WEST  = 3
        };

// used for neighbour cells of pixelFace->nCells[0..1]
enum neighbDirection {
    NEIGHB_SW = 0,
    NEIGHB_NE = 1
            };

// cell types
enum cellTypes {
    PIXEL = 4
        };

struct sFace;
struct sCell;
struct sData;

//------------------------------------------------------
struct sFace {
    sFace() {}

    // grid settings
    double	xy[2];			// x,y-coordinate
    double	deltaxy[2];		// dx,dy relative to x,y
    sCell*	nCells[2];		// two neighbour cells
};

//------------------------------------------------------
struct sCell {
    sCell() :	cType(0),
    noFaces(0),
    cFaces(0),
    nCells(0) {}

    // grid settings
    int	cType;			// cell type
    double	xy[2];			// cell center
    int	noFaces;		// number of faces
    sFace** cFaces;			// cell faces
    sCell** nCells;			// neighbour cells

};

//------------------------------------------------------
struct sData {
    sData() : cells(0), faces(0) {}

    // cells
    int	cellNo;
    sCell*	cells;

    // faces
    int	faceNo;
    sFace*	faces;
};

#endif
