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
#include <stdio.h>
#include <iostream>

#include "data.h"
#include "input.h"
#include "setup.h"
#include "solve.h"
#include "output.h"

//------------------------------------------------------
int main(void)
{
  sData* data = new sData;
  char* cfgFilePath;
  int errLine = 0;

  std::cout << "\tSimulationstechnik V \n\t====================\n";
  cfgFilePath = (char*)"cfg.txt";

  // read config from input file
  if(!input(cfgFilePath,data, errLine)) {
      std::cout << "ERROR while reading input file in line " << errLine << " ...exiting";
      getchar();
      return 1;
  }


  // setup data (boundaries, initial data, etc.)
  if(!setup(data)) {
      std::cout << "ERROR while data setup...exiting";
      getchar();
      return 1;
  }

  // iterativ solver
  if(!solve(data)) {
      std::cout << "ERROR while solving...exiting";
      getchar();
      return 1;
  }
  if(!postprocessing(data)){
      std::cout << "Error while postprocessing...exiting";
      getchar();
      return 1;
  }

  // output data
  if(!output(data)) {
      std::cout << "ERROR while data output...exiting";
      getchar();
      return 1;
  }



  std::cout << "\n################\n#              #\n#   Finished   #"
      "\n#              #\n################\n";
  delete data;
  return 0;
}
