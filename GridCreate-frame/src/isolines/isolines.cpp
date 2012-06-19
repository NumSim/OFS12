/* Contour lines for function evaluated on a grid.

Adapted to use in lecture "Simulationstechnik V"
Copyright (C) 2010 Klaus Leppkes

Copyright (C) 2007, 2008 Kai Habel
Copyright (C) 2004, 2007 Shai Ayal

Adapted to an oct file from the stand alone contourl by Victro Munoz
Copyright (C) 2004 Victor Munoz

Based on contour plot routine (plcont.c) in PLPlot package
http://plplot.org/

Copyright (C) 1995, 2000, 2001 Maurice LeBrun
Copyright (C) 2000, 2002 Joao Cardoso
Copyright (C) 2000, 2001, 2002, 2004  Alan W. Irwin
Copyright (C) 2004  Andrew Ross

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <vector>
#include <algorithm>

#include "isolines.h"
#include "data.h"
#include "input.h"

#define DBL_EPSILON 1e-15
typedef unsigned int uint;

LINE actLine;
LINE *isoLines=0;
int isoLinesCount=0;
int isoLinesSize=0;


void start_contour (double lvl, double x, double y) {
    actLine.x1=x;
    actLine.y1=y;
}

void add_point(double x, double y) {
    actLine.x2=x;
    actLine.y2=y;
    if(isoLinesCount==isoLinesSize) {
        isoLinesSize+=100;
        isoLines=(LINE*)realloc(isoLines,sizeof(LINE)*isoLinesSize);
    }
    isoLines[isoLinesCount++]=actLine;
    actLine.x1=actLine.x2;
    actLine.y1=actLine.y2;
}

char ** allocCharMatrix(int rows, int cols) {
        char **ret=new char*[rows];
        for(int i=0;i<rows;i++) {
                ret[i]=new char[cols];
                for(int j=0;j<cols;j++)
                        ret[i][j]=0;
        }
        return ret;
}

void
drawcn (double  **X, double **Y, double **Z, double lvl,
           int r, int c, double ct_x, double ct_y, uint start_edge, bool first,
           char **mark, int rows, int cols)
{
  double px[4], py[4], pz[4], tmp;
  uint stop_edge, next_edge, pt[2];
  int next_r, next_c;

  px[3] = X[r + 1][ c];
  px[2] = X[r + 1][ c+1];
  px[1] = X[r][ c+1];
  px[0] = X[r][ c];

  py[3] = Y[r + 1][ c];
  py[2] = Y[r + 1][ c+1];
  py[1] = Y[r][ c+1];
  py[0] = Y[r][ c];

  pz[3] = Z [r + 1][ c] - lvl;
  pz[2] = Z [r + 1][ c + 1] - lvl;
  pz[1] = Z [r][ c + 1] - lvl;
  pz[0] = Z [r][ c] - lvl;

  // facet edge and point naming assignment
  //  0-----1   .-0-.
  //  |     |   |   |
  //  |     |   3   1
  //  |     |   |   |
  //  3-----2   .-2-.

  // get mark value of current facet
  char id = static_cast<char>(mark[r][ c]);

  //check startedge s
  if (start_edge == 255)
    {
      //find start edge
      for (uint k = 0; k < 4; k++) {
        if (static_cast<char>(pow(2, k)) & id)
          start_edge = k;
      }
    }

  if (start_edge == 255)
    return;

  //decrease mark value of current facet for start edge
  mark[r][ c] -= static_cast<char>(pow(2, start_edge));

  // next point (clockwise)
  pt[0] = start_edge;
  pt[1] = (pt[0] + 1) % 4;

  //calculate contour segment start if first of contour
  if (first)
    {
      tmp = fabs(pz[pt[1]])/fabs(pz[pt[0]]);
      if (isnan(tmp))
      {
        ct_x = ct_y = 0.5;
      }
      else
      {
        ct_x = px[pt[0]] + (px[pt[1]] - px[pt[0]])/(1 + tmp);
        ct_y = py[pt[0]] + (py[pt[1]] - py[pt[0]])/(1 + tmp);
      }
      start_contour (lvl, ct_x, ct_y);
    }

  //find stop edge FIXME: control flow --> while
  for (uint k = 1; k <= 4; k++)
    {
      if ((start_edge==0) || (start_edge==2))
        stop_edge = (start_edge + k) % 4;
      else
        stop_edge = (start_edge - k) % 4;
      if (static_cast<char>(pow(2, stop_edge)) & id)
        break;
    }

  pt[0] = stop_edge;
  pt[1] = (pt[0] + 1) % 4;
  tmp = fabs(pz[pt[1]])/fabs(pz[pt[0]]);
  if (isnan(tmp))
  {
    ct_x = ct_y = 0.5;
  }
  else
  {
    ct_x = px[pt[0]] + (px[pt[1]] - px[pt[0]])/(1 + tmp);
    ct_y = py[pt[0]] + (py[pt[1]] - py[pt[0]])/(1 + tmp);
  }
  // add point to contour
  add_point (ct_x, ct_y);

  //decrease id value of current facet for start edge
  mark[r][ c] -= static_cast<char>(pow(2,stop_edge));

  //find next facet
  next_c = c;
  next_r = r;

  if (stop_edge == 0)
    next_r--;
  else if (stop_edge == 1)
    next_c++;
  else if (stop_edge == 2)
    next_r++;
  else if (stop_edge == 3)
    next_c--;

  //check if next facet is not done yet
  //go to next facet
  if ((next_r >= 0) && (next_c >= 0) && (next_r < rows) && (next_c < cols))
    if (mark[next_r][ next_c] > 0)
      {
        next_edge = (stop_edge + 2) % 4;
        drawcn (X, Y, Z, lvl, next_r, next_c, ct_x, ct_y, next_edge, false, mark, rows, cols);
      }
}


void mark_facets(double **Z, char **mark, double lvl, int rows, int cols)
{
  //uint c, r, nr = mark.rows(), nc = mark.cols();
  uint c, r, nr = rows, nc = cols;
  double f[4];

  for (c = 0; c < nc; c++)
    for (r = 0; r < nr; r++)
      {
        f[0] = Z[r][ c] - lvl;
        f[1] = Z[r ][c + 1] - lvl;
        f[3] = Z[r + 1][ c] - lvl;
        f[2] = Z[r + 1][ c + 1] - lvl;

        for (uint i = 0; i < 4; i++)
          if (fabs(f[i]) < DBL_EPSILON)
            f[i] = DBL_EPSILON;

        if (f[1] * f[2] < 0)
          mark[r][ c] += 2;
        if (f[0] * f[3] < 0)
          mark[r][ c] += 8;
      }

  for (r = 0; r < nr; r++)
    for (c = 0; c < nc; c++)
      {
        f[0] = Z[r][ c] - lvl;
        f[1] = Z[r][ c + 1] - lvl;
        f[3] = Z[r + 1][ c] - lvl;
        f[2] = Z[r + 1][ c + 1] - lvl;

        for (uint i = 0; i < 4; i++)
          if (fabs(f[i]) < DBL_EPSILON)
            f[i] = DBL_EPSILON;

        if (f[0] * f[1] < 0)
          mark[r][ c] += 1;
        if (f[2] * f[3] < 0)
          mark[r][ c] += 4;
      }
}

void contour (double **X, double **Y, double **Z, double lvl, int rows, int cols)
{
  //uint r, c, nr = Z.rows(), nc = Z.cols();
  uint r, c, nr = rows, nc = cols;
  //charMatrix mark (nr - 1, nc - 1, 0);
  char** mark=allocCharMatrix(rows-1,cols-1);

  mark_facets(Z, mark, lvl, rows-1, cols-1);
  // find contours that start at a domain edge
  for (c = 0; c < nc - 1; c++)
    {
      //top
      if (mark[0][ c] & 1)
      {
        drawcn (X, Y, Z, lvl, 0, c, 0.0, 0.0, 0, true, mark, rows-1, cols-1);
      }
      //bottom
      if (mark[nr - 2][ c] & 4)
      {
        drawcn (X, Y, Z, lvl, nr - 2, c, 0.0, 0.0, 2, true, mark, rows-1, cols-1);
      }
    }

  for (r = 0; r < nr - 1; r++)
    {
      //left
      if (mark[r][ 0] & 8)
      {
        drawcn (X, Y, Z, lvl, r, 0, 0.0, 0.0, 3, true, mark, rows-1, cols-1);
      }
      //right
      if (mark[r][ nc - 2] & 2)
      {
        drawcn (X, Y, Z, lvl, r, nc - 2, 0.0, 0.0, 1, true, mark, rows-1, cols-1);
      }
    }

  for (r = 0; r < nr - 1; r++)
    for (c = 0; c < nc - 1; c++)
      if (mark [r][ c] > 0)
      {
        drawcn (X, Y, Z, lvl, r, c, 0.0, 0.0, 255, true, mark, rows-1, cols-1);
      }
}

void genISOLines(const sData *data, double **potential, LINE* &ISOLines, int &nISOLines, int &nISOcount, int nISO) {

    //LINE *ISOLines=0;
    //int nISOLines=0;
    //int nISOcount=0;

    isoLines=ISOLines;
    isoLinesCount=nISOcount;
    isoLinesSize=nISOLines;

    std::vector<double> isoheights;
    //assemble all boundaryvalues for iso heights
    for (int i=0;i<data->dimX;i++) {
        isoheights.push_back( potential[i][0]);
        //isoheights.push_back( potential[i][data->dimY-1]);
    }
    for (int j=0;j<data->dimY;j++) {
        isoheights.push_back( potential[0][j]);
        //isoheights.push_back( potential[data->dimX-1][j]);
    }

    std::vector<double>::iterator it;
    if(nISO==0) {
        //make iso-height unique
        sort( isoheights.begin(), isoheights.end());
        it = unique (isoheights.begin(), isoheights.end());
        isoheights.resize( it - isoheights.begin() );
    }
    else  {
        //int nISO=10;
        double min=*min_element(isoheights.begin(),isoheights.end());
        double max=*max_element(isoheights.begin(),isoheights.end());
        double delta=(max-min)/nISO;

        isoheights.clear();
        for(int i=0;i<nISO;i++) {
            isoheights.push_back( (min+delta*i) );
        }
    }

    for(it=isoheights.begin();it!=isoheights.end();it++)  {
        double isoheight=*it;
        contour(data->x, data->y, potential, isoheight, data->dimX, data->dimY);
    }

    ISOLines=isoLines;
    nISOcount=isoLinesCount;
    nISOLines=isoLinesSize;
}

bool lineIntersection(LINE &a, LINE &b, double &xout, double &yout) {
        /*
         * Parameterform:
         * (1-t1)*x11+t1*x12=(1-t2)*x21+t2*x22
         * (1-t1)*y11+t1*y12=(1-t2)*y21+t2*y22]
         *
         * t1=(x11*(y21-y22)+x21*(y22-y11)+x22*(y11-y21))/(x11*(y21-y22)+x12*(y22-y21)+(x21-x22)*(y12-y11));
         * t2=-(x11*(y12-y21)+x12*(y21-y11)+x21*(y11-y12))/(x11*(y21-y22)+x12*(y22-y21)+(x21-x22)*(y12-y11)));
         */
        double &x11=a.x1; double &y11=a.y1;
        double &x12=a.x2; double &y12=a.y2;

        double &x21=b.x1; double &y21=b.y1;
        double &x22=b.x2; double &y22=b.y2;


        double z1= (x11*(y21-y22)+x21*(y22-y11)+x22*(y11-y21));
        double z2=-(x11*(y12-y21)+x12*(y21-y11)+x21*(y11-y12));
        double n=(x11*(y21-y22)+x12*(y22-y21)+(x21-x22)*(y12-y11));

        //double z1= x11*y21-x11*y22+x21*y22-x21*y11+x22*y11-x22*y21;
        //double z2=-(x11*y12-x11*y21+x12*y21-x12*y11+x21*y11-x21*y12);
        //double n=x11*y21-x11*y22+x12*y22-x12*y21+(x21-x22)*(y12-y11);

        //if(z1==0) return false;
        //const double eps=1e-10;
        //if( fabs(z1) < eps) return false;
        //if( fabs(z2) < eps) return false;
        //if( fabs( fabs(z1) - fabs(z2)) < 1e-13) return false;

        //
        //if( fabs(z1/z2-1) < eps) return false;

        const double eps=1e-15;

        double t1= z1/n;
        double t2= z2/n; ///(x11*(y21-y22)+x12*(y22-y21)+(x21-x22)*(y12-y11)));

        if( (t1>eps) & (t1<(1-eps)) & (t2>eps) & (t2<(1-eps)) )
        {
                xout = (1-t1)*x11+t1*x12;
                yout = (1-t1)*y11+t1*y12;
                //printf("z1=%lf z2=%lf n=%lf x=%lf y=%lf\n",z1,z2,n, xout, yout);
                return true;
        }
        else
                return false;

}

void NaivLineCrossings(LINE *lines1, int n, LINE* lines2, int m) {
    FILE *f=fopen("cross.txt","w");
    for(int i=0;i<n;i++) {
        for(int j=0;j<m;j++) {
            double x; double y;
            if(lineIntersection(lines1[i], lines2[j],x,y)) {
                fprintf(f,"%lf %lf\n", x,y);
            }
        }
    }
    fclose(f);
}

bool readIsoLines(char *filename, LINE* &lines, int &size, int &counter) {
    FILE *f=fopen(filename,"r");
    double xneu; double yneu;
    LINE l1;

    while(fscanf(f,"%lf %lf",&xneu, &yneu)==2) {
        int length=yneu;
        size+=length-1;
        lines=(LINE*)realloc(lines, sizeof(LINE)*size);

        for(int i=0;i<length;i++) {
            if(fscanf(f,"%lf %lf\n",&xneu, &yneu)!=2) return false;  //fehler beim einlesen
            printf("xneu=%lf yneu=%lf\n",xneu,yneu);
            switch(i) {
                case 0:
                    l1.x1=xneu; l1.y1=yneu; break;
                case 1:
                    l1.x2=xneu; l1.y2=yneu; break;
                default:
                    l1.x1=l1.x2; l1.y1=l1.y2;
                    l1.x2=xneu; l1.y2=yneu; break;
            }
            if(i>0) {
                //printf("neue Linie von (%lf,%lf)->(%lf,%lf)\n", l1.x1, l1.y1, l1.x2, l1.y2);
                lines[counter++]=l1;
            }
        }
    }
    fclose(f);
    return true;
}
