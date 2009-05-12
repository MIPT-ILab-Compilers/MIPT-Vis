/**
 * File: GUI/GuiPoint.cpp - Implementation of GuiPoint in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#include "gui_impl.h"

/**
 * Constructor for class GuiPoint. 
 */
GuiPoint::GuiPoint( GuiEdge * edge, QGraphicsScene * scene):
        QObject(),
        QGraphicsItem( NULL, scene),
        edge( edge),
        isFixed( false)
{
    setMyColor( Qt::red);
    setMyAdjust( 1);
    setZValue( 0);
    setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
    setEnd( 0);
    setStart( 0);
}

/**
 * Constructor for class GuiPoint. 
 */
GuiPoint::GuiPoint( GuiEdge * edge, QGraphicsItem * parent, QGraphicsScene * scene):
        QObject(),
        QGraphicsItem( parent, scene),
        edge( edge),
        isFixed( false)
{
//    setFlag( QGraphicsItem::ItemStacksBehindParent);
    setMyColor( Qt::red);
    setMyAdjust( 1);
    setZValue( 0);
    setEnd( 0);
    setStart( 0);
}
/**
 * 
 */
void GuiPoint::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if( edge != NULL)
    {
            edge->updatePosition();
    }
    update();
    QGraphicsItem::mousePressEvent( mouseEvent);
}

/**
 * 
 */
void GuiPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent * event)
{
    if( edge != NULL)
    {
            edge->updatePosition();
    }
    update();
    QGraphicsItem::mouseReleaseEvent( event);
}

/**
 * 
 */
void GuiPoint::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    QGraphicsItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Add edge for the point
 */
void GuiPoint::addEdge( GuiEdge * line)
{
    edges << line;
}

/**
 * If point's position changed this function run
 */
QVariant GuiPoint::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if( change == QGraphicsItem::ItemPositionChange)
    {
        if( edge != NULL)
        {
            edge->updatePosition();
        }
    }
    return value;
}

/**
 * Draw point
 */
void GuiPoint::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->fillRect( boundingRect(), QBrush( myColor));
    myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
                      << (boundingRect().topRight()) << (boundingRect().topLeft())
                      << (boundingRect().bottomLeft());
}

/**
 * Get bounding rectangle of the point
 */
QRectF GuiPoint::boundingRect() const
{
    return QRect( -5,-5, 10,10);
}

/**
 * Get shape of the point
 */
QPainterPath GuiPoint::shape() const
{
    QPainterPath path;
    path.addRect( -5, -5, 10, 10);
    return path; 
}
/**
 * Update position of the point
 */
void GuiPoint::updatePosition()
{
    if( parentItem() != NULL)
    {
        setPos( parentItem()->boundingRect().center());
    }
}