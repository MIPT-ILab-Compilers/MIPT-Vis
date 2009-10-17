/**
 * File: GUI/GuiEdge.cpp - Implementation of GuiEdge in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include <QtGui/QGraphicsLineItem>
class GuiPoint;

/**
 * Constructor of GuiEdge class
 */
GuiEdge::GuiEdge(  GuiGraph * graph_p, int _id, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent, QGraphicsScene * scene)
            :QGraphicsLineItem( parent, scene), Edge( ( Graph *)( graph_p), _id, 
            static_cast< Node *>( startItem), static_cast< Node *>( endItem))
{
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    startPoint = 0;
    endPoint = 0;
    setFlag( QGraphicsItem::ItemIsSelectable, true);
}

/**
 * Update position the points of the edge
 */
void GuiEdge::updatePoints()
{
    if( startPoint != NULL && startPoint->end() != NULL)
    {
        GuiPoint * next = this->startPoint->end()->end();
        for( int i = 1; i <= this->pointsNum(); i++)
        {
            if( next != NULL)
            {
                this->point( i)->x = next->pos().x();
                this->point( i)->y = next->pos().y();
            }
            if( next->end() != NULL)
            {
                next = next->end()->end();
            }
        }
    }
}

/**
 * Update position the edge
 */
void GuiEdge::updatePosition()
{
    foreach( GuiPoint * point, points)
    {
        point->updatePosition();
        if( point->end() != NULL)
            point->end()->updatePosition();
        if( point->start() != NULL)    
            point->start()->updatePosition();
    } 
    update();
}

/**
 * Add point to the edge
 */
void GuiEdge::addPoint( GuiPoint * point)
{
    points << point;
}

/**
 * Init start and end points for the edge. Also init edgePart  between points
 */
void GuiEdge::initNode( GuiNode * startItem, GuiNode * endItem)
{
    startPoint = new GuiPoint( this, startItem, scene());
    endPoint = new GuiPoint( this, endItem, scene());
    startPoint->setPos( startItem->QGraphicsItem::pos());
    startPoint->setInit();
    endPoint->setPos( endItem->QGraphicsItem::pos());
    endPoint->setInit();
    GuiEdgePart * ep = new GuiEdgePart( this, startPoint, endPoint, scene());
    edgePart << ep;
    startPoint->setEnd( ep);
    endPoint->setStart( ep);
    addPoint( startPoint);
    addPoint( endPoint);
    startPoint->setFixed();
    endPoint->setFixed();
}

/**
 * 
 */
void GuiEdge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsLineItem::mousePressEvent( event);
}

/**
 * 
 */
void GuiEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsLineItem::mouseReleaseEvent( event);
}

/**
 * 
 */
void GuiEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsLineItem::mouseDoubleClickEvent( event);
}

/**
 * 
 */
void GuiEdge::addEdgePart( GuiEdgePart * part)
{
    edgePart << part;
}

/**
 * 
 */
void GuiEdge::showPoints()
{
    foreach( GuiPoint * point, points)
    {
        if ( point != startPoint
             && point != endPoint)
        {
            point->setFixed( false);
            point->update();
        }
    }
}

/**
 * 
 */
void GuiEdge::hidePoints()
{
    foreach( GuiPoint * point, points)
    {
        point->setFixed( true);
        point->update();
    }
}

