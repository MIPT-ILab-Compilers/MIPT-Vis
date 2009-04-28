/**
 * File: GUI/GuiNode.cpp - Implementation of GuiNode in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

/**
 * When forus in the text of the node function run
 */
void GuiNode::focusInEvent( QFocusEvent * event)
{
    setSelected( true);
    QGraphicsTextItem::focusInEvent( event);
}

/**
 * When forus out the text of the node function run
 */
void GuiNode::focusOutEvent( QFocusEvent * event)
{
    setTextInteractionFlags( Qt::NoTextInteraction);
    setSelected( false);
    QGraphicsTextItem::focusOutEvent( event);
}

/**
 * Edit text in the node after double click mouse
 */
void GuiNode::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if ( mouseEvent->button() & Qt::LeftButton)
    {
        if ( textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags( Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * 
 */
void GuiNode::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    QGraphicsTextItem::mousePressEvent( mouseEvent);
}

/**
 * 
 */
void GuiNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsTextItem::mouseReleaseEvent( event);
}

/**
 * Draw rectangle around the node
 */
void GuiNode::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setPen( QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->fillRect( boundingRect(), QBrush( myColor));
    QGraphicsTextItem::paint( painter, option, widget);
    myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
                      << (boundingRect().topRight()) << (boundingRect().topLeft())
                      << (boundingRect().bottomLeft());
}

/**
 * Get bounding rectangle of the node
 */
QRectF GuiNode::boundingRect() const
{
    return QGraphicsTextItem::boundingRect()
               .adjusted( -myAdjust, -myAdjust, myAdjust, myAdjust);
}

/**
 * Add edge for the node
 */
void GuiNode::addEdge( GuiEdge * line)
{
    edges << line;
}

/**
 * If node's position is changed, run this function
 */
QVariant GuiNode::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if( change == QGraphicsItem::ItemPositionChange)
    {
        foreach( GuiEdge * line, edges)
        {
            if( line != NULL)
            {
                line->updatePosition();
            }
        }
    }
    return value;
}

/**
 *  Write to xml
 */
void GuiNode::writeByXMLWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterStartElement( writer, BAD_CAST "node");

	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%d", id());
    xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "x", "%d", (int)QGraphicsItem::x());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "y", "%d", (int)QGraphicsItem::y());
    xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "width", "%d", (int)(GuiNode::boundingRect()).width());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "height", "%d", (int)(GuiNode::boundingRect()).height());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST label());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST color());
    xmlTextWriterWriteAttribute( writer, BAD_CAST "shape", BAD_CAST NodeProperties::shape());
	
	xmlTextWriterEndElement( writer);
    xmlTextWriterWriteString( writer, BAD_CAST "\n");
}