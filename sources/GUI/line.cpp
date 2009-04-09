/**
 * File: GUI/line.cpp - Implementation of edge in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"
#include <QtGui/QGraphicsLineItem>
const qreal Pi = 3.14;

/**
 * 
 */
Line::Line(Text * startItem, Text * endItem, QGraphicsItem * parent, QGraphicsScene * scene):QGraphicsLineItem(parent, scene)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag( QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::red;
    setPen( QPen( myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

/**
 * 
 */
QRectF Line::boundingRect() const
{
    qreal extra = ( pen().width() + 20) / 1.0;
    return QRectF( line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted( -extra, -extra, extra, extra);
}

/**
 * 
 */
QPainterPath Line::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon( lineHead);
    return path;
}

/**
 * 
 */
void Line::updatePosition()
{
    QLineF line( mapFromItem( myStartItem, (myStartItem->boundingRect().center())), 
        mapFromItem( myEndItem, (myEndItem->boundingRect().center())));
    setLine( line);
}

/**
 * 
 */
void Line::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if ( myStartItem->collidesWithItem( myEndItem))
        return;
    QPen myPen = pen();
    myPen.setColor( myColor);
    qreal arrowSize = 10;
    painter->setPen( myPen);
    painter->setBrush( myColor);
    QLineF centerLine( mapFromItem( myStartItem, (myStartItem->boundingRect().center())), 
        mapFromItem( myEndItem, (myEndItem->boundingRect().center())));
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for ( int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon.at(i) + myEndItem->pos();
        polyLine = QLineF( p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect( centerLine, &intersectPoint);
        if ( intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }
    setLine( QLineF( intersectPoint, mapFromItem( myStartItem, (myStartItem->boundingRect().center()))));
    double angle = ::acos( line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;
    QPointF lineP1 = line().p1() + QPointF( sin(angle + Pi / 3) * arrowSize, cos( angle + Pi / 3) * arrowSize);
    QPointF lineP2 = line().p1() + QPointF( sin(angle + Pi - Pi / 3) * arrowSize, cos( angle + Pi - Pi / 3) * arrowSize);
    lineHead.clear();
    lineHead << line().p1() << lineP1 << lineP2;
    painter->drawLine( line());
    painter->drawPolygon( lineHead);
    if (isSelected())
    {
        painter->setPen( QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate( 0, 4.0);
        painter->drawLine( myLine);
        myLine.translate( 0,-8.0);
        painter->drawLine( myLine);
    }
}
