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
    startPoint = 0;
    endPoint = 0;
    setFlag( QGraphicsItem::ItemIsSelectable, true);
}
/**
 * Update position the edge
 */
void GuiEdge::updatePosition()
{
    foreach( GuiPoint * point, points)
    {
        point->updatePosition();
        if( point->end() !=NULL)
            point->end()->updatePosition();
        if( point->start() !=NULL)    
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
 * 
 */
void GuiEdge::initPoint( GuiNode * startItem, GuiNode * endItem)
{
    startPoint = new GuiPoint( this, startItem, scene());
    endPoint = new GuiPoint( this, endItem, scene());
    startPoint->setPos( startItem->boundingRect().center());
    endPoint->setPos( endItem->boundingRect().center());
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

/**
 * Write edge points by xml writer
 */
void GuiEdge::writePointsByXMLWriter( xmlTextWriterPtr writer)
{
	int i;
	for ( i = 1; i <= pointsNum(); i++)
	{
		xmlTextWriterWriteString( writer, BAD_CAST "\t\t");
		xmlTextWriterStartElement( writer, BAD_CAST "point");

		xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "n", "%d", i);
		xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "x", "%d", point(i)->x);
		xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "y", "%d", point(i)->y);
		
		xmlTextWriterEndElement( writer);
		xmlTextWriterWriteString( writer, BAD_CAST "\n");
	}
}

/**
 * Write edge by xml writer
 */
void GuiEdge::writeByXMLWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterStartElement( writer, BAD_CAST "edge");

	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%d", id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "from", "%d", pred()->id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "to", "%d", succ()->id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "points_num", "%d", pointsNum());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST label());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "prob", "%d", prob());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "thickness", "%d", thickness());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST color());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST style());

    xmlTextWriterWriteString( writer, BAD_CAST "\n");
	writePointsByXMLWriter( writer);

	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterEndElement( writer);
    xmlTextWriterWriteString( writer, BAD_CAST "\n");
}
