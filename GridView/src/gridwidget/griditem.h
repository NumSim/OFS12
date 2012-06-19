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
#ifndef FLOWGridItem_H
#define FLOWGridItem_H

#include <QtGui>

#include "data.h"

class GridItem : public QGraphicsItem {
private:
        const sData* mp_data;
        qreal m_Scale;
        qreal m_Radius;

        int m_noFaceLines;
        QLineF* mp_FaceLines;
        QRectF m_FacesRect;
        qreal m_FaceLineMean;

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
        GridItem(sData* data);
	QRectF boundingRect() const;
};

#endif
