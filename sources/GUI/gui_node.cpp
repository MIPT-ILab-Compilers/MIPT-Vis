/**
 * File: GUI/GuiNode.cpp - Implementation of GuiNode in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

/**
 * Constructor of GuiNode class
 */
GuiNode::GuiNode(  QString * text, GuiGraph * graph_p, int _id, 
        QGraphicsItem * parent, QGraphicsScene * scene):
	    myText(),
        myAdjust(0),
        QGraphicsTextItem( parent, scene),
        NodeAux( static_cast<GraphAux *> ( graph_p), _id)
{
	QGraphicsItem::setCursor(Qt::ArrowCursor);
	setPlainText( *text);
	setTextWidth ( 100); //Set width of node
	setMyAdjust (real()? 3 : 1);
	setMyColor( Qt::green);
	setFlag( QGraphicsItem::ItemIsMovable, true); // Set node can move
	setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
	setTextInteractionFlags( Qt::NoTextInteraction);
	myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
				  << (boundingRect().topRight()) << (boundingRect().topLeft())
				  << (boundingRect().bottomLeft());
}
/**
 * Destructor of GuiNode class
 */
GuiNode::~GuiNode()
{
	out ("last nitems:%d, deletenig: ", addGui (getGraph())->items().count());
	debugPrint();
	addGui(getGraph())->removeItem (addGui(this));
	out ("current nitems%d", addGui (getGraph())->items().count());
}

/**
 * When forus in the text of the node function run
 */
void GuiNode::focusInEvent( QFocusEvent * event)
{
    setSelected( true);
    QGraphicsTextItem::focusInEvent( event);
}

/**
 * When forus out the text of the node function run
 */
void GuiNode::focusOutEvent( QFocusEvent * event)
{
    setTextInteractionFlags( Qt::NoTextInteraction);
    setSelected( false);
    QGraphicsTextItem::focusOutEvent( event);
}

/**
 * Edit text in the node after double click mouse
 */
void GuiNode::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if ( mouseEvent->button() & Qt::LeftButton)
    {
        if ( textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags( Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * 
 */
void GuiNode::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    QGraphicsTextItem::mousePressEvent( mouseEvent);
}

/**
 * 
 */
void GuiNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsTextItem::mouseReleaseEvent( event);
}

/**
 * Draw rectangle around the node
 */
void GuiNode::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if ( real())
	{
	    painter->setPen( QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		painter->fillRect( boundingRect(), QBrush( myColor));
		QGraphicsTextItem::paint( painter, option, widget);
		myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
		                  << (boundingRect().topRight()) << (boundingRect().topLeft())
		                  << (boundingRect().bottomLeft());
	}
	else
	{
		painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		painter->fillRect( boundingRect(), QBrush( myColor));
		myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
		                  << (boundingRect().topRight()) << (boundingRect().topLeft())
		                  << (boundingRect().bottomLeft());
	}
}

/**
 * Get bounding rectangle of the node
 */
QRectF GuiNode::boundingRect() const
{
	if (!real()) return QRectF (0, 0, 15, 15);//!!! magic namber
    return QGraphicsTextItem::boundingRect()
               .adjusted( -myAdjust, -myAdjust, myAdjust, myAdjust);
}


/**
 * If node's position is changed, run this function
 */
QVariant GuiNode::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if( change == QGraphicsItem::ItemPositionChange 
		|| change == QGraphicsItem::ItemPositionHasChanged)
    {
        setX( QGraphicsItem::x());
        setY( QGraphicsItem::y());
        setWidth( boundingRect().width());
        setHeight( boundingRect().height());
        
		EdgeAux* iter;
		ForEdges(this, iter, Succ)
			addGui(iter)->updatePosition();
		ForEdges(this, iter, Pred)
			addGui(iter)->updatePosition();

    }
    return value;
}

/**
 *  commitPos
 */
void GuiNode::commitPos( int x,int y)
{
    setPos( x,y);
    setX( x);
    setY( y);

	EdgeAux* iter;
	ForEdges(this, iter, Succ)
		addGui(iter)->updatePosition();
	ForEdges(this, iter, Pred)
		addGui(iter)->updatePosition();
}

/**
 *  superscribe
 */
void GuiNode::superscribe (QColor color, QString text)
{
    setMyColor( color);
    setPlainText( text);
}
/**
 *  setMyText
 */
void GuiNode::setMyText(const QString & str)
{
    myText = str;
}
