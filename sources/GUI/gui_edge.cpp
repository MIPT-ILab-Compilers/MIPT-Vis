/**
 * File: GUI/GuiEdge.cpp - Implementation of GuiEdge in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include <QtGui/QGraphicsLineItem>
//class GuiPoint;

/**
 * Constructor of GuiEdge class
 */
GuiEdge::GuiEdge(  GuiGraph * graph_p, int _id, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent, QGraphicsScene * scene)
            :QGraphicsItem( parent, scene), EdgeAux( ( GraphAux *)( graph_p), _id, 
            static_cast< NodeAux *>( startItem), static_cast< NodeAux *>( endItem))
{
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    setFlag( QGraphicsItem::ItemIsSelectable, true);
}
/**
 * Destructor of GuiEdge class
 */
GuiEdge::~GuiEdge()
{
	out ("last nitems:%d, deletenig: ", addGui (getGraph())->items().count());
	debugPrint();
	addGui (getGraph())->removeItem (addGui (this));
	out ("current nitems%d", addGui (getGraph())->items().count());
}

/**
 * Update position the edge
 */
void GuiEdge::updatePosition()
{
	startP = mapFromItem( addGui (pred()), pred()->width()/2, pred()->height()/2);
    endP = mapFromItem( addGui (succ()), succ()->width()/2, succ()->height()/2);//!!!rarely it not work
    topLeft.setX( min< qreal>( startP.x(), endP.x()));
    topLeft.setY( min< qreal>( startP.y(), endP.y()));
    btmRight.setX( max< qreal>( startP.x(), endP.x()));
    btmRight.setY( max< qreal>( startP.y(), endP.y())); 
    update();
}

QRectF GuiEdge::boundingRect() const
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
QPainterPath GuiEdge::shape() const
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
void GuiEdge::paint( QPainter * painter,
                         const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if (succ() == 0 || pred() == 0)//!!! What is the ugliness!
	{
		out ("ERROR: the deleted edge is tryed to paint!");
		return;
	}
    qreal arrowSize = 10;
    painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->setBrush( Qt::black);
    painter->drawLine( startP, endP);
	GuiNode* suc = addGui (succ());
	if( suc->real())
    {
        QLineF centerLine( startP, endP);
        QPolygonF endPolygon = suc->polygon();
        QPolygonF lineHead;
        QPointF p1 = endPolygon.first() + suc->pos();
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for ( int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + suc->pos();
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
    update();
}

void GuiEdge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent( event);
}

/**
 * 
 */
void GuiEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent( event);
}

/**
 * 
 */
void GuiEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseDoubleClickEvent( event);
	insertNode (event->pos());
}

GuiNode* GuiEdge::insertNode (QPointF p)
{
	GuiNode* node = addGui (graph->insertNodeOnEdge (this));
	
	node->setReal( false);
	node->update();
	node->setPos (p.x(), p.y());
	return node;
}
