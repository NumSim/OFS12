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
#include <math.h>

#include "griditem.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

GridItem::GridItem(sData *data)
    : mp_data(data)
{
    qreal minX = 0., maxX = 1.;
    qreal minY = 0., maxY = 1.;

    // loop over cells and their faces to set bounding box
    m_FaceLineMean = 0.0;
    if(data->faceNo > 0) {
        minX = data->faces[0].xy[0];
        maxX = data->faces[0].xy[0];
        minY = data->faces[0].xy[0];
        maxY = data->faces[0].xy[0];
        for (int f = 0; f < data->faceNo; f++) {
            sFace* face = &(data->faces[f]);
            if(face->xy[0] < minX) minX = face->xy[0];
            if(face->xy[0] > maxX) maxX = face->xy[0];
            if(face->xy[1] < minY) minY = face->xy[1];
            if(face->xy[1] > maxY) maxY = face->xy[1];
            if(face->xy[0] +face->deltaxy[0] < minX) minX = face->xy[0] +face->deltaxy[0];
            if(face->xy[0] +face->deltaxy[0] > maxX) maxX = face->xy[0] +face->deltaxy[0];
            if(face->xy[1] +face->deltaxy[1] < minY) minY = face->xy[1] +face->deltaxy[1];
            if(face->xy[1] +face->deltaxy[1] > maxY) maxY = face->xy[1] +face->deltaxy[1];
            m_FaceLineMean = m_FaceLineMean +sqrt(face->deltaxy[0]*face->deltaxy[0] +face->deltaxy[1]*face->deltaxy[1]);
        }
        m_FaceLineMean = m_FaceLineMean /mp_data->faceNo; 
    }

    m_Scale = 100.0/m_FaceLineMean;
    m_Radius = m_FaceLineMean /10. *m_Scale;

    m_FacesRect.setX(minX*m_Scale);
    m_FacesRect.setY(minY*m_Scale);
    m_FacesRect.setWidth((maxX-minX)*m_Scale);
    m_FacesRect.setHeight((maxY-minY)*m_Scale);

    // precalc FaceLines
    m_noFaceLines = data->faceNo;
    mp_FaceLines = new QLineF[m_noFaceLines];
    sFace* face = 0;
    for (int f = 0; f < data->faceNo; f++) {
        face = &(mp_data->faces[f]);
        mp_FaceLines[f].setLine( face->xy[0]*m_Scale, face->xy[1]*m_Scale,
                                (face->xy[0]+face->deltaxy[0])*m_Scale, (face->xy[1]+face->deltaxy[1])*m_Scale );
    }

}

void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QFont font = painter->font();
    font.setPointSize(m_FaceLineMean*m_Scale/10.);
    painter->setFont(font);

    // fill gradient
    QLinearGradient gradient(m_FacesRect.topLeft(), m_FacesRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(m_FacesRect, gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_FacesRect);

    // draw faces
    painter->drawLines(mp_FaceLines,m_noFaceLines);

    // draw faceids
    sFace* face = 0;
    qreal arrowSize = 10.;
    for (int f = 0; f < mp_data->faceNo; f++) {
        face = &(mp_data->faces[f]);
        QPointF center( (face->xy[0]+(face->deltaxy[0]/2.0))*m_Scale,
                        (face->xy[1]+(face->deltaxy[1]/2.0))*m_Scale );

        QRadialGradient gradient(center, m_Radius);
        gradient.setColorAt(0, QColor(Qt::yellow).lighter(150));
        gradient.setColorAt(1, QColor(Qt::darkYellow).lighter(150));

        painter->setBrush(gradient);
        painter->setPen(QPen(Qt::black, 0));
        painter->drawEllipse(center, m_Radius, m_Radius);

        painter->setPen(Qt::black);
        QRectF rect( center-QPointF(m_Radius,m_Radius),QSizeF(2.*m_Radius, 2.*m_Radius) );
        painter->drawText(rect, Qt::AlignCenter, QString("%1").arg(f) );

        QPointF direction( (face->xy[0]+(face->deltaxy[0]/3.0))*m_Scale,
                           (face->xy[1]+(face->deltaxy[1]/3.0))*m_Scale );
        double angle = acos(mp_FaceLines[f].dx() / mp_FaceLines[f].length());
        if (mp_FaceLines[f].dy() >= 0) angle = TwoPi - angle;
        QPointF sourceArrowP1 = direction -QPointF(sin(angle + Pi / 3) * arrowSize,
                                                      cos(angle + Pi / 3) * arrowSize);
        QPointF sourceArrowP2 = direction -QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                      cos(angle + Pi - Pi / 3) * arrowSize);
        painter->setBrush(Qt::black);
        painter->drawPolygon(QPolygonF() << direction << sourceArrowP1 << sourceArrowP2);
    }

    // draw cellids
    sCell* cell = 0;
    for (int c = 0; c < mp_data->cellNo; c++) {
        cell = &(mp_data->cells[c]);
        QPointF center( cell->xy[0]*m_Scale, cell->xy[1]*m_Scale );

        QRadialGradient gradient(center, m_Radius);
        gradient.setColorAt(0, QColor(Qt::red).lighter(150));
        gradient.setColorAt(1, QColor(Qt::darkRed).lighter(150));

        painter->setBrush(gradient);
        painter->setPen(QPen(Qt::black, 0));
        painter->drawEllipse(center, m_Radius, m_Radius);

        painter->setPen(Qt::black);
        QRectF rect( center-QPointF(m_Radius,m_Radius),QSizeF(2.*m_Radius, 2.*m_Radius) );
        painter->drawText(rect, Qt::AlignCenter, QString("%1").arg(c) );
    }
}

QRectF GridItem::boundingRect() const
{
    QRectF bRect(m_FacesRect.x()-m_Radius,m_FacesRect.y()-m_Radius,
                 m_FacesRect.width() + 2*m_Radius, m_FacesRect.height() + 2*m_Radius);
    return bRect;
}
