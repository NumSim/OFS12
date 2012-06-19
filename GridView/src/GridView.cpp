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

#include <QtGui>
#include <QMessageBox>

#include "gridwidget.h"
#include "input.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sData* data = new sData;
    const char* faceFilePath = "/home/thomas/workspace/OFS12/GridCreate-frame/run/faces2.cfg";
    const char* cellFilePath = "/home/thomas/workspace/OFS12/GridCreate-frame/run/cells2.cfg";

  //  QDir::setCurrent(
  //         QFileDialog::getExistingDirectory( 0, QString("Set Grid-Files Directory"), QString()) );

    // read config from input file
    if(!input(faceFilePath, cellFilePath, data)) {
        QMessageBox::critical( 0, QString("Grid-Files not found"),
                                  QString("ERROR while reading input files...exiting"));
            return 1;
    }

    GridWidget widget(data);
    widget.show();
    return app.exec();
}
