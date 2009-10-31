/**
 * File: GUI/GuiEdgePart.cpp - Implementation of GuiEdgePart in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

/**
 * Constructor fo GuiEdgePart class
 */
GuiEdgePart::GuiEdgePart( GuiEdge * e, GuiPoint * start, GuiPoint * end, QGraphicsScene * scene): 
    QGraphicsItem( NULL, scene),
    edge( e)
{
    QGraphicsItem::setCursor(Qt::ArrowCursor);
	assert( start != NULL);
    assert( end != NULL);
    setZValue( -1000);
    setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
    start->setEnd( this);
    end->setStart( this);
	startPoint = start;
	endPoint = end;
    update();
}

/**
 * 
 */
QRectF GuiEdgePart::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(topLeft,
                   QSizeF( btmRight.x() - topLeft.x(),
                           btmRight.y() - topLeft.y()))
           .normalized()
           .adjusted(-adjust, -adjust, adjust, adjust);
}

/**
 * 
 */
QPainterPath GuiEdgePart::shape() const
{
    QPainterPath path( startP);
    QPainterPathStroker stroker;
    path.lineTo( endP.x(), endP.y());
    stroker.setWidth( 10);
     return stroker.createStroke( path);
}

/**
 * Paint arrow
 */
void GuiEdgePart::paint( QPainter * painter,
                         const QStyleOptionGraphicsItem * option, QWidget * widget)
{ 
    qreal arrowSize = 10;
    painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->setBrush( Qt::black);
    painter->drawLine( startP, endP);
    QGraphicsItem * ep = scene()->itemAt( endP.x() + 20, endP.y());
    if( ep != NULL)
    {
        if( ep->type() == GuiNode::Type)
        {
            GuiNode * ed = qgraphicsitem_cast<GuiNode *> ( ep);
            QLineF centerLine( startP, endP);
            QPolygonF endPolygon = ed->polygon();
            QPolygonF lineHead;
            QPointF p1 = endPolygon.first() + ed->pos();
            QPointF p2;
            QPointF intersectPoint;
            QLineF polyLine;
            for ( int i = 1; i < endPolygon.count(); ++i)
            {
                p2 = endPolygon.at(i) + ed->pos();
                polyLine = QLineF( p1, p2);
                QLineF::IntersectType intersectType = polyLine.intersect( centerLine, &intersectPoint);
                if ( intersectType == QLineF::BoundedIntersection)
                {
                    break;
                }
                p1 = p2;
            }
            QLineF line( intersectPoint, startP);
            double angle = ::acos( line.dx() / line.length());
            if ( line.dy() >= 0)
            {
                angle = ( Pi * 2) - angle;
            }
            QPointF lineP1 = line.p1() + QPointF( sin( angle + Pi / 3) * arrowSize, cos( angle + Pi / 3) * arrowSize);
            QPointF lineP2 = line.p1() + QPointF( sin( angle + Pi - Pi / 3) * arrowSize, cos( angle + Pi - Pi / 3) * arrowSize);
            lineHead.clear();
            lineHead << line.p1() << lineP1 << lineP2;
            painter->drawLine( line);
            painter->drawPolygon( lineHead);
        }
    }
    update();
}

/**
 * 
 */
void GuiEdgePart::mousePressEvent( QGraphicsSceneMouseEvent * event)
{
    update();
    QGraphicsItem::mousePressEvent( event);
}

/**
 * 
 */
void GuiEdgePart::mouseReleaseEvent( QGraphicsSceneMouseEvent * event)
{
    update();
    QGraphicsItem::mouseReleaseEvent( event);
}

/**
 * 
 */
void GuiEdgePart::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event)
{
    if ( edge != NULL 
         && event->button() & Qt::LeftButton)
    { 
        addPoint( event->pos());
//        edge->showPoints();
    }
    setSelected( false);
    update();
    QGraphicsItem::mouseDoubleClickEvent( event);
}

/**
 * Update position for edgepart
 */
void GuiEdgePart::updatePosition()
{
    startP = mapFromItem( startPoint, 0, 0);
    endP = mapFromItem( endPoint, 0, 0);
    topLeft.setX( min< qreal>( startP.x(), endP.x()));
    topLeft.setY( min< qreal>( startP.y(), endP.y()));
    btmRight.setX( max< qreal>( startP.x(), endP.x()));
    btmRight.setY( max< qreal>( startP.y(), endP.y()));
    update();
}

/**
 * Add point to the edge part
 */
GuiPoint * GuiEdgePart::addPoint( QPointF p)
{
    GuiPoint * point = new GuiPoint( edge, scene());
    point->setPos( p);
    point->setInit();
//    GuiEdgePart* seg = new GuiEdgePart( edge, point, end(), scene()); 
//    seg->updatePosition();
//    setEnd( point);
    setSelected( false);
    if ( edge != NULL)
    {
//        int n = edge->pointsNum() + 1;
  //      edge->addPoint( point);
  //      edge->addEdgePart( seg);
  //      edge->initPoints( n + 1);
  //      edge->setPoint( new EdgePoint(), n);
  //      edge->point( n)->x = p.x();
		//edge->point( n)->y = p.y();
        updatePosition();
//        edge->updatePoints();
    }
    return point;
}