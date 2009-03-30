/**
 * File: GUI/rectangle.h - Implementational header for rectangle of GUI in MiptVis tool.
 * Copyright (C) 2009  Ivan Mashintsev
 */
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QtGui/QGraphicsRectItem>

/**
 * Class Rectangle. Node is movable. Double click create new node
 */
class Rectangle:public QGraphicsRectItem
{
public:
    Rectangle( QGraphicsItem * parent = 0);
};
#endif
