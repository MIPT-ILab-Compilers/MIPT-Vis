/**
 * File: GUI/GuiNode.cpp - Implementation of GuiNode in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include<QtGui/QAction>
#include<QtGui/QMenu>

/**
 * Init NodeProperties
 */
NodeProperties::NodeProperties (StyleSheet* ss_) :ss_priv (ss_), style_priv (ss_->getId ("default"))
{
    color_priv = 0;
	label_priv = 0;
	shape_priv = 0;
	text_priv = 0;
}
/**
 * Constructor of GuiNode class
 */
GuiNode::GuiNode(  QString * text, GuiGraph * graph_p, int _id, StyleSheet* ss,
        QGraphicsItem * parent, QGraphicsScene * scene):
		NodeProperties (ss),
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
	applStyle (painter, option);
	if ( real())
	{
		painter->drawRect( boundingRect());
		QGraphicsTextItem::paint( painter, option, widget);
		myPolygon << ( boundingRect().bottomLeft()) << ( boundingRect().bottomRight())
		                  << ( boundingRect().topRight()) << ( boundingRect().topLeft())
		                  << ( boundingRect().bottomLeft());
	}
	else
	{
		if (!addGui (graph)->showVnodes()) return;//do not draw virtual nodes
		painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		painter->drawRect (boundingRect());
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
        setWidth( boundingRect().width());
        setHeight( boundingRect().height());
        setX( QGraphicsItem::x() + width()/2);
        setY( QGraphicsItem::y() + height()/2);
        
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
    setX( x);
    setY( y);

    setPos( x - width() / 2, y - height() / 2);

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
//    setMyColor( color);
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
	if (0 != stName().compare ("default", Qt::CaseInsensitive))
		xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST stName().toAscii().data());
}

/**
 *  Read from xml
 */
void GuiNode::readByXML( xmlNode * cur_node)
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
		else if ( xmlStrEqual( props->name, xmlCharStrdup( "style")))
		{
			setStyle( ( char *)( props->children->content));
		}
	}
}

/**
 *  contextMenuEvent
 */
void GuiNode::contextMenuEvent( QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
    QAction *deleteAct = menu.addAction( "Delete");
	connect( deleteAct, SIGNAL( triggered()), this, SLOT( emitDelete()));
	menu.exec( event->screenPos());
}

/**
 *  emit delete
 */
void GuiNode::emitDelete()
{
	emit deleteMe( this->userId());
}
