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
#include <string.h>
#include <math.h>

#include "input.h"
#include "data.h"

//------------------------------------------------------
bool input(const char* faceFilePath, const char* cellFilePath, sData* data)
{
    int section, lineNo;
    sCell* curCell=0;
    sFace* curFace=0;
    char line[256]=" ", token[16]=" ";

    // face vars
    int faceId;
    double x		= MAXDOUBLE;
    double y		= MAXDOUBLE;
    double deltax       = MAXDOUBLE;
    double deltay	= MAXDOUBLE;

    // cell vars
    int cellId;
    int cType;
    int f[10];

    double nsCheck, weCheck;

    /////////////////////
    // READ FACE FILE  //
    /////////////////////

    // open face file
    std::ifstream faceFile(faceFilePath);
    if(!faceFile) { return false; }

    // read face data
    section=0;
    lineNo=0;
    while(!faceFile.eof()) {
        lineNo++;
        faceFile.getline(line,255);
        if(sscanf(line,"%15s",token)<1) { continue; };

        if(!strcmp(token,"#")) {
            // skip comment lines
        }else if(!strcmp(token,"faces")) {
            if(sscanf(line,"%15s %d",token,&(data->faceNo))	!= 2){ return error(faceFilePath,lineNo,line); };
            data->faces = new sFace[data->faceNo];
            section=1;
        }else if(!strcmp(token,"initvalues")) {
            if(sscanf(line,"%15s",token)	                != 1) { return error(cellFilePath,lineNo,line); }
            section=2;
        }else if(!strcmp(token,"boundaries")) {
            if(sscanf(line,"%15s",token)			!= 1){ return error(faceFilePath,lineNo,line); };
            section=3;
        }else if(section==1) {	// reading grid section
            if(sscanf(line,"%d %lf %lf %lf %lf",&faceId,&x,&y,&deltax,&deltay)	!= 5){ return error(faceFilePath,lineNo,line); };
            curFace=&data->faces[faceId];
            curFace->xy[0] = x;
            curFace->xy[1] = y;
            curFace->deltaxy[0] = deltax;
            curFace->deltaxy[1] = deltay;
            curFace->nCells[0] = 0;
            curFace->nCells[1] = 0;
        }else if(section==2) { 	// reading initvalue section

        }else if(section==3) {	// reading boundary section

        }
    }
    faceFile.close();

    /////////////////////
    // READ CELL FILE  //
    /////////////////////

    // open cell file
    std::ifstream cellFile(cellFilePath);
    if(!cellFile) { return false; }

    // read cell data
    section=0;
    lineNo=0;
    while(!cellFile.eof()) {
        lineNo++;
        cellFile.getline(line,255);
        if(sscanf(line,"%15s",token)<1){ continue; };

        if(!strcmp(token,"#")) {
            // skip comment lines
        }else if(!strcmp(token,"cells")) {
            if(sscanf(line,"%15s %d",token,&(data->cellNo))		!= 2) { return error(cellFilePath,lineNo,line); }
            data->cells = new sCell[data->cellNo];
            section=1;
        }else if(!strcmp(token,"initvalues")) {
            if(sscanf(line,"%15s",token)             			!= 1) { return error(cellFilePath,lineNo,line); }
            section=2;
        }else if(!strcmp(token,"boundaries")) {
            if(sscanf(line,"%15s",token)						!= 1) { return error(cellFilePath,lineNo,line); }
            section=3;
        }else if(section==1) { 	// reading grid section
            if(sscanf(line,"%d %d",&cellId,&cType)				!= 2) { return error(cellFilePath,lineNo,line); }
            curCell=&data->cells[cellId];
            curCell->cType	=cType;
            switch(curCell->cType) {
            case 4:
                if(sscanf(line,"%d %d %lf %lf %d %d %d %d",
                          &cellId,&cType,&curCell->xy[0],&curCell->xy[1],
                          &f[0],&f[1],&f[2],&f[3]) != 8) { return error(cellFilePath,lineNo,line); }
                curCell->noFaces= 4;
                curCell->cFaces	= new sFace*[4];
                for(int l=0; l<4; l++) {curCell->cFaces[l]=0;}

                // search for position of face on the cell
                for(int l=0; l<4; l++) {
                    curFace = &data->faces[f[l]];

                    // sort faces in south, east, north, west
                    // sort faces in south, east, north, west
                    nsCheck = ABS(curFace->deltaxy[0]);
                    weCheck = ABS(curFace->deltaxy[1]);
                    if(nsCheck > 0) {
                        if (curCell->cFaces[SOUTH]) {
                            if (curCell->cFaces[SOUTH]->xy[1] > curFace->xy[1]) {
                                curCell->cFaces[NORTH] = curCell->cFaces[SOUTH];
                                curCell->cFaces[SOUTH] = curFace;
                            } else {
                                curCell->cFaces[NORTH] = curFace;
                            }
                        } else {
                            curCell->cFaces[SOUTH] = curFace;
                        }
                    } else if (weCheck > 0) {
                        if (curCell->cFaces[WEST]) {
                            if (curCell->cFaces[WEST]->xy[0] > curFace->xy[0]) {
                                curCell->cFaces[EAST] = curCell->cFaces[WEST];
                                curCell->cFaces[WEST] = curFace;
                            } else {
                                curCell->cFaces[EAST] = curFace;
                            }
                        } else {
                            curCell->cFaces[WEST] = curFace;
                        }
                    } else {
                        std::cout << "Position of face " << f[l] << " in cell " << cellId << " undefined." << std::endl;
                        return error(cellFilePath,lineNo,line);
                    }
                }
                // check if for all faces a position got found
                for(int l=0; l<4; l++) {
                    if(!curCell->cFaces[l]) {
                        std::cout << "Face " << l << " of cell " << cellId << " undefined." << std::endl;
                        return error(cellFilePath,lineNo,line);
                    }
                }
                break;
            }
        }else if(section==2) { 	// reading initvalue section

        }else if(section==3) { 	// reading boundary section

        }
    }
    cellFile.close();

    return true;
}

//------------------------------------------------------
bool error(const char* filePath, int lineNo, const char* line)
{
    std::cout << "ERROR reading " << filePath << ", line " << lineNo << ":" << std::endl;
    std::cout << "\t" << line << std::endl << std::endl;
    return false;
}
