/**
 * File: GUI/GuiNode.cpp - Implementation of GuiNode in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

/**
 * Init NodeProperties
 */
NodeProperties::NodeProperties()
{
    color_priv = 0;
	label_priv = 0;
	shape_priv = 0;
	text_priv = 0;
}
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
	QGraphicsItem::setCursor( Qt::ArrowCursor);
	setPlainText( *text);
	setTextWidth ( 100); //Set width of node
	NodeAux::setWidth( 100);
	setMyAdjust( real()? 3 : 1);
	setMyColor( Qt::green);
	setMyText( "");
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
	out( "last nitems:%d, deletenig: ", addGui( getGraph())->items().count());
	debugPrint();
	addGui( getGraph())->removeItem( addGui(this));
	out( "current nitems%d", addGui( getGraph())->items().count());
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
 * Actions for mouse press event
 */
void GuiNode::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    QGraphicsTextItem::mousePressEvent( mouseEvent);
}

/**
 * Actions for mouse release event
 */
void GuiNode::mouseReleaseEvent( QGraphicsSceneMouseEvent *event)
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
		myPolygon << ( boundingRect().bottomLeft()) << ( boundingRect().bottomRight())
		                  << ( boundingRect().topRight()) << ( boundingRect().topLeft())
		                  << ( boundingRect().bottomLeft());
	}
	else
	{
		painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		painter->fillRect( boundingRect(), QBrush( myColor));
		myPolygon << ( boundingRect().bottomLeft()) << ( boundingRect().bottomRight())
		                  << ( boundingRect().topRight()) << ( boundingRect().topLeft())
		                  << ( boundingRect().bottomLeft());
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
        setX( QGraphicsItem::x() + width()/2);
        setY( QGraphicsItem::y() + height()/2);
        setWidth( boundingRect().width());
        setHeight( boundingRect().height());
        
		EdgeAux* iter;
		ForEdges( this, iter, Succ)
			addGui(iter)->updatePosition();
		ForEdges( this, iter, Pred)
			addGui( iter)->updatePosition();

    }
    return value;
}

/**
 *  commitPos
 */
void GuiNode::commitPos( int x, int y)
{
    setPos( x - width() / 2, y - height() / 2);
    setX( x);
    setY( y);

	EdgeAux* iter;
	ForEdges( this, iter, Succ)
		addGui( iter)->updatePosition();
	ForEdges( this, iter, Pred)
		addGui( iter)->updatePosition();
}

/**
 *  superscribe
 */
void GuiNode::superscribe ( QColor color, QString text)
{
    setMyColor( color);
    setPlainText( text);
}

/**
 *  setMyText
 */
void GuiNode::setMyText( const QString & str)
{
    myText = str;
}

/**
 *  textChange
 */
void GuiNode::textChange()
{
    QByteArray strByteArray = getMyText().toAscii();
    char *strChar;
    strChar = ( char*) calloc( strByteArray.size(),sizeof( char));
    if ( strChar==NULL) return;
    int i;
    for ( i=0; i<=strByteArray.size(); ++i) strChar[i] = strByteArray[i];
    setTextPriv( strChar);
}

/**
 *  Write to xml
 */
void GuiNode::writeByXMLWriter( xmlTextWriterPtr writer)
{
	NodeAux::writeByXMLWriter ( writer);
	if ( label()) xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST label());
	if ( color()) xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST color());
	if ( NodeProperties::shape())
		xmlTextWriterWriteAttribute( writer, BAD_CAST "shape", BAD_CAST NodeProperties::shape());
	if ( textPriv()) xmlTextWriterWriteAttribute( writer, BAD_CAST "textPriv", BAD_CAST textPriv());
}

/**
 *  Read from xml
 */
void GuiNode::readByXML (xmlNode * cur_node)
{
	NodeAux::readByXML ( cur_node);
	setTextPriv( "");
	for ( xmlAttr* props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup( "color")))
		{
			setColor( ( char *)( props->children->content));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "label")))
		{
			setLabel( ( char *)( props->children->content));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "shape")))
		{
			setShape( ( char *)( props->children->content));
		}
		else if ( xmlStrEqual( props->name, xmlCharStrdup( "textPriv")))
		{
			setTextPriv( ( char *)( props->children->content));
		}
	}
}
