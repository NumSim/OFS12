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
#include <fstream>
#include <string.h>
#include <math.h>
#include <iostream>

#include "input.h"
#include "data.h"

//------------------------------------------------------
bool input(const char* cfgFilePath, sData* data, int &errLine)
{
  std::cout << "\nInput:\t------->\t";

  ///////////////////////
  // READ CONFIG FILE  //
  ///////////////////////
  errLine = 0;
  int lineNo;
  char line[256]=" ", token[16]=" ";

  // open input file
  std::ifstream cfgFile(cfgFilePath);
  if(!cfgFile) { return false; }

  // read input file line by line
  lineNo=0;
  while(!cfgFile.eof()) {
      lineNo++;
      cfgFile.getline(line,255);

      if(sscanf(line,"%15s",token)<1) { continue; };    // skip empty lines
      if(!strcmp(token,"#")) {                                          // skip comment lines
      } else if(!strcmp(token,"dimI")) {
          if(sscanf(line,"%15s %d",token,&data->dimI)   != 2){ errLine = lineNo; return false; };
      } else if(!strcmp(token,"dimJ")) {
          if(sscanf(line,"%15s %d",token,&data->dimJ)   != 2){ errLine = lineNo; return false; };
      } else if(!strcmp(token,"deltaXi")) {
          if(sscanf(line,"%15s %lf",token,&data->deltaXi)       != 2){ errLine = lineNo; return false; };
      } else if(!strcmp(token,"deltaEta")) {
          if(sscanf(line,"%15s %lf",token,&data->deltaEta)      != 2){ errLine = lineNo; return false; };
      } else if(!strcmp(token,"maxIter")) {
          if(sscanf(line,"%15s %d",token,&data->maxIter)        != 2){ errLine = lineNo; return false; };
      } else if(!strcmp(token,"residuum")) {
          if(sscanf(line,"%15s %lf",token,&data->residuum)!= 2){ errLine = lineNo; return false; };
      } else if(!strcmp(token,"overrelax")) {
          if(sscanf(line,"%15s %lf",token,&data->overrelax)!= 2){ errLine = lineNo; return false; };
      }else if(!strcmp(token,"finiteDiffDx")) {
          if(sscanf(line,"%15s %lf",token,&data->finiteDiffDx)!= 2){ errLine = lineNo; return false; };
      }else if(!strcmp(token,"finiteDiffDy")) {
          if(sscanf(line,"%15s %lf",token,&data->finiteDiffDy)!= 2){ errLine = lineNo; return false; };
      }else {
          std::cout << "unknown token: " << token << std::endl;
          return false;
      }
  }
  cfgFile.close();

  data->x               = allocGrid1Mem(data, MAXDOUBLE);
  data->y               = allocGrid1Mem(data, MAXDOUBLE);
  data->s1      = allocGrid1Mem(data, MAXDOUBLE);
  data->xi      = allocGrid1Mem(data,MAXDOUBLE);
  data->eta     = allocGrid1Mem(data,MAXDOUBLE);
  std::cout << "Success!\n";
  return true;
}
