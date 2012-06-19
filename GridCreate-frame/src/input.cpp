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
#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "input.h"
#include "data.h"

//------------------------------------------------------
bool input(const char* cfgFilePath, sData* data)
{
	char line[256]=" ", token[16]=" ";
	char strFilePath[256], potFilePath[256];

	std::cout << "\nInput:\n-------\n";

	//////////////////////
	// READ JOB FILE    //
	//////////////////////

	// open input file
	std::ifstream cfgFile(cfgFilePath);
	if(!cfgFile) { return false; }

	// read input file line by line
	while(!cfgFile.eof()) {
		cfgFile.getline(line,255);
		if(!sscanf(line,"%15s",token)){ continue; };

		if(!strcmp(token,"#")) {
			// skip comment lines
		} else if(!strcmp(token,"dimX")) {
			sscanf(line,"%15s %d",token,&data->dimX);
		} else if(!strcmp(token,"dimY")) {
			sscanf(line,"%15s %d",token,&data->dimY);
		} else if(!strcmp(token,"sizeX")) {
			sscanf(line,"%15s %lf",token,&data->sizeX);
		} else if(!strcmp(token,"sizeY")) {
			sscanf(line,"%15s %lf",token,&data->sizeY);
		} else if(!strcmp(token,"strfile")) {
			sscanf(line,"%15s %s",token, strFilePath);
		} else if(!strcmp(token,"potfile")) {
			sscanf(line,"%15s %s",token, potFilePath);
		} else {
			std::cout << "unknown token: " << token << std::endl;		
		}
	}
	cfgFile.close();

	/////////////////////////////////////
	// READ STREAM/POTENTIAL DATAFILE  //
	/////////////////////////////////////
	// read_dset(strFilePath, data, data->s1);
	// read_dset(potFilePath, data, data->s2);

	/////////////////////////////////////
	// COPY VALUES AND ALLOCATE MEMORY //
	/////////////////////////////////////
	data->x = allocGrid1Mem(data, MAXDOUBLE);
	data->y	= allocGrid1Mem(data, MAXDOUBLE);

	return true;
}

//------------------------------------------------------
bool read_dset(const char* filePath, sData* data, double** s)
{
	int i=0, j=0;
	std::string strline;
	char *cstr, *token;

	std::cout << "\n    reading " << filePath << std::endl;
	std::ifstream file(filePath);
	if(!file) {
		std::cout << "cannot open file: " << filePath << std::endl;
		return false;
	}

	i=0; j=0;
	s = allocGrid1Mem(data, MAXDOUBLE);
	while(!file.eof()) {
		std::getline(file, strline);

		cstr = new char [strline.size()+1];
		strcpy(cstr, strline.c_str());

		token = strtok(cstr," ");
		while( token != NULL ) {
			sscanf(token,"%lf", &s[i][j]);
			token = strtok( NULL," ");
			j++;
		}
		i++;
		delete[] cstr;
	} 
	file.close();
	return true;
}
