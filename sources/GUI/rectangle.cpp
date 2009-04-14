/**
 * File: GUI/rectangle.cpp - Implementation of rectangle in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"


/**
 * Constructor for class Rectangle. 
 */
Rectangle::Rectangle( QGraphicsItem * parent, QGraphicsScene * scene):Text( new QString(""), parent, scene)
{
    setPlainText( "");
    setMyColor( Qt::blue);
    setTextWidth ( 22); //Set width of node
    setMyAdjust( -5);
    setFlag( QGraphicsItem::ItemIsMovable, true); // Set node can move
    setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
    setTextInteractionFlags( Qt::NoTextInteraction);
}

/**
 * 
 */
void Rectangle::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    QGraphicsTextItem::mousePressEvent( mouseEvent);
}

/**
 * 
 */
void Rectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    update();
    QGraphicsTextItem::mouseReleaseEvent( event);
}

/**
 * 
 */
void Rectangle::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Remove line for the rectangle
 */
void Rectangle::removeLine( Line * line)
{
    int index = lines.indexOf( line);
    if ( index != -1)
    {
        lines.removeAt( index);
    }
}

/**
 * Remove all lines for the rectangle
 */
void Rectangle::removeLines()
{
    foreach ( Line * line, lines)
    {
        line->startItem()->removeLine( line);
        line->endItem()->removeLine( line);
        scene()->removeItem( line);
        delete line;
    }
}

/**
 * Add line for the rectangle
 */
void Rectangle::addLine( Line * line)
{
    lines.append( line);
}

/**
 * If rectangle's position changed this function run
 */
QVariant Rectangle::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if( change == QGraphicsItem::ItemPositionChange)
    {
        foreach( Line * line, lines)
        {
            line->updatePosition();
        }
    }
    return value;
}