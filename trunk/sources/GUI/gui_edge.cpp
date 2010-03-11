/**
 * File: GUI/GuiEdge.cpp - Implementation of GuiEdge in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include <QtGui/QGraphicsLineItem>

/**
 * Constructor of GuiEdge class
 */
GuiEdge::GuiEdge(  GuiGraph * graph_p, int _id, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent, QGraphicsScene * scene)
            :QGraphicsItem( parent, scene), EdgeAux( ( GraphAux *)( graph_p), _id, 
            static_cast< NodeAux *>( startItem), static_cast< NodeAux *>( endItem))
{
    QGraphicsItem::setCursor( Qt::ArrowCursor);
    setFlag( QGraphicsItem::ItemIsSelectable, true);
}
/**
 * Destructor of GuiEdge class
 */
GuiEdge::~GuiEdge()
{
	out( "last nitems:%d, deletenig: ", addGui ( getGraph())->items().count());
	debugPrint();
	addGui( getGraph())->removeItem( addGui( this));
	out( "current nitems%d", addGui( getGraph())->items().count());
}


QLineF::IntersectType getIntersection (const QLineF& l, const QPolygonF& p, QPointF* intersectPoint)
{
    QPointF p1 = p.first();
    QPointF p2;
    QLineF polyLine;
    for ( int i = 1; i < p.count(); ++i)
    {
        p2 = p.at(i);
        polyLine = QLineF( p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect( l, intersectPoint);
        if ( intersectType == QLineF::BoundedIntersection)
        {
            return QLineF::BoundedIntersection;
        }
        p1 = p2;
    }
	return QLineF::NoIntersection;
}

/**
 * Update position the edge
 */
void GuiEdge::updatePosition()
{
	GuiNode* pre = addGui (pred());
	GuiNode* suc = addGui (succ());

	startP = mapFromItem( pre, pre->width()/2, pre->height()/2);
    endP = mapFromItem( suc, suc->width()/2, suc->height()/2);//!!!rarely it not work

	QPolygonF headPolygon = suc->polygon();
	headPolygon.translate (suc->pos());
	QPolygonF tailPolygon = pre->polygon();
	tailPolygon.translate (pre->pos());

	getIntersection (QLineF (startP, endP), headPolygon, &endP);
	getIntersection (QLineF (startP, endP), tailPolygon, &startP);

	QPointF delta = startP - endP;
	delta.setX(0);

	startDir = startP - delta/2;
	endDir = endP + delta/2;

    topLeft.setX( min< qreal>( startP.x(), endP.x()));
    topLeft.setY( min< qreal>( startP.y(), endP.y()));
    btmRight.setX( max< qreal>( startP.x(), endP.x()));
    btmRight.setY( max< qreal>( startP.y(), endP.y())); 
    update();
}

/**
 * Return bounding rectangle
 */
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
 * Shape
 */
QPainterPath GuiEdge::shape() const
{
    QPainterPath path( startP);
    QPainterPathStroker stroker;
    path.lineTo( endP.x(), endP.y());
    stroker.setWidth( 10);
    return stroker.createStroke( path);
}

void drawLineHead (QPainter * painter, QPointF end, double angle, double size)
{
    QPointF lineP1 = end + QPointF( sin( angle + Pi / 3) * size, cos( angle + Pi / 3) * size);
    QPointF lineP2 = end + QPointF( sin( angle + Pi - Pi / 3) * size, cos( angle + Pi - Pi / 3) * size);

    QPolygonF lineHead;
    lineHead.clear();
    lineHead << end << lineP1 << lineP2;
	
    painter->drawPolygon (lineHead);
}

/**
 * Paint arrow
 */
void GuiEdge::paint( QPainter * painter,
                         const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if ( succ() == 0 || pred() == 0)//!!! What is the ugliness!
	{
		out ("ERROR: the deleted edge is tried to paint!");
		return;
	}
    qreal arrowSize = 10;
    painter->setPen( QPen( Qt::darkRed, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
//    painter->drawLine( startP, endP);
	GuiNode* suc = addGui ( succ());
	if( suc->real())
    {
		QPointF dir = (7*endDir + startP)/8 - endP;//!!! Mnemonic rule, it must be changed
		
		painter->setBrush( Qt::darkRed);
		drawLineHead (painter, endP, -atan2 (dir.y(), dir.x()), 10);

		painter->setBrush( Qt::transparent);//!!! change it to black, and you will see, what heppend. I can't explain this
    }
    painter->setPen( QPen( Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter->drawPoint (startDir);
	painter->drawPoint (endDir);

	QPainterPath path;
	path.moveTo (startP);
	path.cubicTo (startDir, endDir, endP);

	painter->drawPath(path);//!!! It is a very damp code, i write it to get understand curves


    update();
}

/**
 * Handle mouse press event
 */
void GuiEdge::mousePressEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent( event);
}

/**
 * Handle mouse release event
 */
void GuiEdge::mouseReleaseEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent( event);
}

/**
 * Handle mouse double click event
 */
void GuiEdge::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseDoubleClickEvent( event);
	insertNode( event->pos());
}

/**
 * Insert node
 */
GuiNode* GuiEdge::insertNode ( QPointF p)
{
	GuiNode* node = addGui ( graph->insertNodeOnEdge( this));
	
	node->setReal( false);
	node->update();
	node->setPos( p.x(), p.y());
	return node;
}
/**
 * Write edge by xml writer
 */
void GuiEdge::writeByXMLWriter( xmlTextWriterPtr writer)
{
	EdgeAux::writeByXMLWriter( writer);
	xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST label());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "prob", "%d", prob());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "thickness", "%d", thickness());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST color());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST style());
}

/**
 *  Read from xml
 */
void GuiEdge::readByXML( xmlNode * cur_node)
{
	EdgeAux::readByXML( cur_node);
	for ( xmlAttr * props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup( "prob")))
		{
			setProb( strtoul( ( const char *)( props->children->content), NULL, 0) );
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "thickness")))
		{
			setThickness( strtoul( ( const char *)( props->children->content), NULL, 0) );
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "color")))
		{
			setColor( ( char *)( props->children->content));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "style")))
		{
			setStyle( ( char *)( props->children->content));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "label")))
		{
			setLabel( ( char *)( props->children->content));
		}
	}
}
