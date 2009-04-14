/**
 * File: GUI/rectangle.h - Implementational header for rectangle of GUI in MiptVis tool.
 * Copyright (C) 2009  Ivan Mashintsev
 */
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QtGui/QGraphicsRectItem>

class Line;
/**
 * Class Rectangle. 
 */
class Rectangle:public Text
{
private:
    QList<Line *> lines;
    QPolygonF myPolygon;
public:
    Rectangle( QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
    void removeLine( Line *line);
    void removeLines();
    void addLine( Line * line);
protected:
    QVariant itemChange( GraphicsItemChange change, const QVariant &value);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
};
#endif
