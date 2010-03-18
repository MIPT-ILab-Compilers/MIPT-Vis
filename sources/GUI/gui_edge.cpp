/**
 * File: GUI/GuiEdge.cpp - Implementation of GuiEdge in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include <QtGui/QGraphicsLineItem>

/**
 * Constructor of GuiEdge class
 */
GuiEdge::GuiEdge(  GuiGraph * graph_p, int _id, StyleSheet* ss, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent, QGraphicsScene * scene)
            :QGraphicsItem( parent, scene), EdgeAux( ( GraphAux *)( graph_p), _id, 
            static_cast< NodeAux *>( startItem), static_cast< NodeAux *>( endItem)),
			EdgeProperties (ss)
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

	start_point = mapFromItem( pre, pre->width()/2, pre->height()/2);
    end_point = mapFromItem( suc, suc->width()/2, suc->height()/2);//!!!rarely it not work

	if (pre == suc)//mesh edge
	{
		QPointF heigh (0, 2*pre->height());

		QPointF middle (pre->pos().x() - 10, pre->pos().y() + pre->height()/2);
		QPointF middleDirUp = middle + heigh;
		QPointF middleDirDown = middle - heigh;

		start_dir = start_point + heigh;
		end_dir = end_point - heigh;

		QPolygonF polygon = suc->polygon();
		polygon.translate (suc->pos());
		getIntersection (QLineF (start_point, start_dir), polygon, &start_point);
		getIntersection (QLineF (end_point, end_dir), polygon, &end_point);

		QPainterPath path;
		path.moveTo (start_point);
		path.cubicTo (start_dir, middleDirUp, middle);
		path.cubicTo (middleDirDown, end_dir, end_point);
		curve = path;
	}
	else
	{
		valid = true;
		
		QPolygonF headPolygon = suc->polygon();
		headPolygon.translate (suc->pos());
		QPolygonF tailPolygon = pre->polygon();
		tailPolygon.translate (pre->pos());


		if (suc->real())
			valid = valid && getIntersection (QLineF (start_point, end_point), headPolygon, &end_point) == QLineF::BoundedIntersection;
		if (pre->real()) 
			valid = valid && getIntersection (QLineF (start_point, end_point), tailPolygon, &start_point) == QLineF::BoundedIntersection;

		QPointF delta = start_point - end_point;
		delta.setX(0);

		if (pre->real()) 
			start_dir = (start_point + end_point)/2;
		else
			start_dir = start_point - delta/2;

		if (suc->real())
			end_dir = (start_point + end_point)/2;
		else
			end_dir = end_point + delta/2;

		QPainterPath path;
		path.moveTo (start_point);
		path.cubicTo (start_dir, end_dir, end_point);

		if (valid) curve = path;
	}

    top_left.setX( min< qreal>( start_point.x(), end_point.x()));
    top_left.setY( min< qreal>( start_point.y(), end_point.y()));
    bottom_right.setX( max< qreal>( start_point.x(), end_point.x()));
    bottom_right.setY( max< qreal>( start_point.y(), end_point.y())); 
    update();
}

/**
 * Return bounding rectangle
 */
QRectF GuiEdge::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(top_left,
                   QSizeF( bottom_right.x() - top_left.x(),
                           bottom_right.y() - top_left.y()))
           .normalized()
           .adjusted(-adjust, -adjust, adjust, adjust);
}

/**
 * Shape
 */
QPainterPath GuiEdge::shape() const
{
//    QPainterPath path( start_point);
    QPainterPathStroker stroker;
//    path.lineTo( end_point.x(), end_point.y());
    stroker.setWidth( 10);
    return stroker.createStroke( curve);
}

void drawLineHead (QPainter * painter, QPointF end, double angle, double size, bool figure)
{
    QPointF lineP1 = end + QPointF( sin( angle + Pi / 3) * size, cos( angle + Pi / 3) * size);
    QPointF lineP2 = end + QPointF( sin( angle + Pi - Pi / 3) * size, cos( angle + Pi - Pi / 3) * size);
	QPointF centre = (lineP1 + lineP2 + end)/3;

	if (figure)
	{
		QPainterPath arrow;
		arrow.moveTo (end);
		arrow.quadTo (centre, lineP1);
		arrow.lineTo (lineP2);
		arrow.quadTo (centre, end);

		painter->drawPath (arrow);
	}
	else
	{
		QPolygonF lineHead;
		lineHead.clear();
		lineHead << end << lineP1 << lineP2;
	
		painter->drawPolygon (lineHead);
	}
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
	if (!valid) return;

	applStyle (painter, option);

    qreal arrowSize = 10;
//    painter->setPen( QPen( Qt::darkRed, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	GuiNode* suc = addGui ( succ());
	GuiNode* pre = addGui ( pred());
	
	if (!pre->real() && pre->firstPred() == 0 && !addGui (graph)->showVnodes()) return;
	if( suc->real())
    {
		QPointF dir = (7*end_dir + start_point)/8 - end_point;//!!! Mnemonic rule, it must be changed
		drawLineHead (painter, end_point, -atan2 (dir.y(), dir.x()), 10, false);
    }
//    painter->setPen( QPen( Qt::darkRed, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
//	painter->drawPoint (start_dir);		//debug drawing
//	painter->drawPoint (end_dir);

	painter->setBrush( Qt::transparent);//!!! change it to black, and you will see, what heppend. I can't explain this
	painter->drawPath (curve);


//    update();
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
GuiNode* GuiEdge::insertNode ( QPointF p)	//!!! I think it's superannuated
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
	if (0 != stName().compare ("default", Qt::CaseInsensitive))
		xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST stName().toAscii().data());
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
