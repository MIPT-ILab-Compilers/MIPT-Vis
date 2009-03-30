/**
 * File: GUI/rectangle.cpp - Implementation of rectangle in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"
#include <QtGui/QtGui>

/**
 * Constructor for class Rectangle. Create node with default properties 
 */
Rectangle::Rectangle( QGraphicsItem * parent):QGraphicsRectItem( parent)
{
    setRect( QRectF( -30, -30, 100, 40)); // Set size of the node
    setFlag( QGraphicsItem::ItemIsMovable, true); // Set rectangle can move
    setFlag( QGraphicsItem::ItemIsSelectable, true); // Set rectangle can select
    setPen( QPen( Qt::black)); // Set color of the frame
    setBrush( QBrush( Qt::green)); // Set background color of the node
    setFlag( QGraphicsItem::ItemClipsChildrenToShape, true); // Set node is movable
}