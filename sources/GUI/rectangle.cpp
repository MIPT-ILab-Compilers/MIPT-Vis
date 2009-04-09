/**
 * File: GUI/rectangle.cpp - Implementation of rectangle in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"


/**
 * Constructor for class Rectangle. Create node with default properties 
 */
Rectangle::Rectangle( QGraphicsItem * parent, QGraphicsScene *scene):Text( parent, scene)
{
    setPlainText( "");
    setTextWidth ( 5); //Set width of node
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
void Rectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsTextItem::mouseReleaseEvent( event);
}

/**
 * Edit text in the node after double click mouse
 */
void Rectangle::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * 
 */
void Rectangle::removeLine( Line *line)
{
    int index = lines.indexOf( line);
    if (index != -1)
        lines.removeAt( index);
}

/**
 * 
 */
void Rectangle::removeLines()
{
    foreach (Line *line, lines)
    {
        line->startItem()->removeLine( line);
        line->endItem()->removeLine( line);
        scene()->removeItem( line);
        delete line;
    }
}

/**
 * 
 */
void Rectangle::addLine(Line *line)
{
    lines.append( line);
}

/**
 * 
 */
QVariant Rectangle::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        foreach( Line *line, lines)
        {
            line->updatePosition();
        }
    }
    return value;
}