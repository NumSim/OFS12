/***************************************************************************
 *   Copyright (C) 2010 by  RWTH Aachen University                         *
 *   klaus.leppkes@rwth-aachen.de                                          *
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
#ifndef ISOLINES_H
#define ISOLINES_H

#include "data.h"

typedef struct _line{
        _line(): x1(0.), y1(0.), x2(0.), y2(0.) {};
        double x1;
        double y1;
        double x2;
        double y2;
} LINE;

void genISOLines(const sData *data, double **potential, LINE* &ISOLines, int &nISOLines, int &nISOcount, int nISO=0);
bool lineIntersection(LINE &a, LINE &b, double &xout, double &yout);

void NaivLineCrossings(LINE *lines1, int n, LINE* lines2, int m);
bool readIsoLines(char *filename, LINE* &lines, int &size, int &counter);

#endif // ISOLINES_H
