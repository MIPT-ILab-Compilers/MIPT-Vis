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
	/*
	else
	{
		commitPos(300,300);
		superscribe(Qt::red,"abc");
	}
	*/
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * 
 */
void GuiNode::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if ( mouseEvent->button() & Qt::LeftButton)
    {
        QString str = toPlainText();
        emit isClicked(str);
    }
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
        setX( QGraphicsItem::x());
        setY( QGraphicsItem::y());
        setWidth( boundingRect().width());
        setHeight( boundingRect().height());
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
 *  commitPos
 */
void GuiNode::commitPos( int x,int y)
{
    setPos( x,y);
    setX( x);
    setY( y);
    foreach( GuiEdge * line, edges)
    {
        if( line != NULL)
        {
            line->updatePosition();
        }
    }
}

/**
 *  superscribe
 */
void GuiNode::superscribe (QColor color,QString text)
{
    setMyColor( color);
    setPlainText( text);
    foreach( GuiEdge * line, edges)
    {
        if( line != NULL)
        {
            line->startPoint->setMyColor(this->myColor);
        }
    }
}
