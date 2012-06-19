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
#include <QDebug>
#include <QWheelEvent>

#include "gridwidget.h"
#include "griditem.h"

GridWidget::GridWidget(sData* data)
{
    // setup view
    setWindowTitle(tr("SimTechV -- GridView"));
    setMinimumSize(600, 600);
    setRenderHint(QPainter::Antialiasing);

    // setup scene
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect( 0.,0.,1000,1000 );
    setScene(scene);
    fitInView( scene->sceneRect(), Qt::KeepAspectRatio);
    scale(qreal(0.7), qreal(0.7));

    // setup item
    GridItem* grid = new GridItem(data);
    qreal GridScale = scene->sceneRect().width()/grid->boundingRect().width();
    grid->scale(GridScale,GridScale);
    scene->addItem(grid);
    grid->setPos(-grid->boundingRect().x()*GridScale,-grid->boundingRect().y()*GridScale);
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        m_mode = Panning;
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void GridWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_mode = None;
    setDragMode(QGraphicsView::NoDrag);
    QGraphicsView::mouseReleaseEvent(event);
}

void GridWidget::wheelEvent(QWheelEvent *event)
{
    qreal factor = pow((double)2, -event->delta() / scene()->width());
    scale(factor,factor);
 }


