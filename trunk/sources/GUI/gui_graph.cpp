/**
 * File: GUI/GuiGraph.cpp - Implementation of GuiGraph in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include "StyleSheet.h"

/**
 * Create element after double click mouse.
 */
void GuiGraph::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if( mouseEvent->button() != Qt::LeftButton)
    {
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
        return;  
    }
    if( selectedItems().isEmpty())
    {
        number++;
        GuiNode * node = ( GuiNode *)newNode();
        node->setPos( mouseEvent->scenePos());
        node->setColor( "green");
		node->setLabel( "Node" + number);
		node->setShape( "rectangle");
		node->setTextPriv( "");        
        node->setWidth( node->boundingRect().width());
        node->setHeight( node->boundingRect().height());
		node->setX( node->QGraphicsItem::x() + node->width()/2);
		node->setY( node->QGraphicsItem::y() + node->height()/2);

		QString text =  QString( "Node %1").arg( node->userId());
		node->setPlainText(text);

		QObject::connect(node, SIGNAL( deleteMe( int)), this, SLOT( deleteNode( int)));
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    }
    QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    
    emit isClicked();
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( QObject * parent):myMode( insertRect), GraphAux(), QGraphicsScene( parent)
{
    number = 0;
    line = NULL;
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( char * filename, QObject * parent):myMode( insertRect), GraphAux(), QGraphicsScene( parent)
{
    GuiNode * node;
    GuiEdge * edge;
    number = 0;
    line = NULL;

    readFromXML( filename);

    for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
	{
        node->setPos( node->NodeAux::x(), node->NodeAux::y());
		node->setMyText(QString( node->textPriv()));
        node->setX( node->QGraphicsItem::x());
        node->setY( node->QGraphicsItem::y());
		node->setMyAdjust( node->real()? 3 : 1);

		QString text =  QString( "Node %1").arg( node->userId());
		node->setPlainText(text);

		QObject::connect(node, SIGNAL( deleteMe( int)), this, SLOT( deleteNode( int)));
	}
	for ( edge = ( GuiEdge *)firstEdge(); isNotNullP( edge); edge = ( GuiEdge *) edge->nextEdge())
	{
        if ( edge != NULL)
        {
		    edge->updatePosition();
        }
	}
}
/**
 * Create line after right button clicked
 */
void GuiGraph::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if( mouseEvent->button() & Qt::RightButton)
    {
        myMode = insertLine;
        line = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen( QPen( Qt::black));
        addItem( line);
    }
    update();
    QGraphicsScene::mousePressEvent( mouseEvent);
    
    emit isClicked();
}

/**
 * Move line when move mouse.
 */
void GuiGraph::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if ( myMode == insertLine && line != 0)
    {
        QLineF newLine( line->line().p1(), mouseEvent->scenePos());
        line->setLine( newLine);
    } 
    else if ( myMode == moveItem)
    {
        QGraphicsScene::mouseMoveEvent( mouseEvent);
    }
}

/**
 * Insert line on the scene after release the mouse.
 */
void GuiGraph::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if ( line != 0 && myMode == insertLine)
    {
        QList<QGraphicsItem *> startItems = items( line->line().p1());
        if ( startItems.count() && startItems.first() == line)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items( line->line().p2());
        if ( endItems.count() && endItems.first() == line)
        {
            endItems.removeFirst();
        }
        removeItem( line);
        delete line;
        if ( startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first() && 
            startItems.first()->type() == GuiNode::Type &&
            endItems.first()->type() == GuiNode::Type)
        {
            GuiNode * startItem = qgraphicsitem_cast< GuiNode *> ( startItems.first());
            GuiNode * endItem = qgraphicsitem_cast< GuiNode *> ( endItems.first());
            GuiEdge * lin = (GuiEdge *)newEdge( startItem, endItem);

        }
    }
    line = NULL;
    myMode = moveItem;
    QGraphicsScene::mouseReleaseEvent( mouseEvent);
    emit isClicked();
}

/**
 * Read style table
 */
void GuiGraph::readAttribsFromXml (xmlNode * a_node)
{
	ss.loadFromXmlNode (a_node);
	GraphAux::readAttribsFromXml (a_node);
}

/**
 * Allocation memory for edge
 */
EdgeAux * GuiGraph::createEdge( Node * pred, Node * succ)
{
    GuiEdge * e = new GuiEdge ( this, incEdgeId(), ( GuiNode *)pred, ( GuiNode *)succ);
    addItem( e);
    e->setStyle( "Solid");
    e->setZValue( -1000.0);
    e->updatePosition();

    return static_cast <EdgeAux*> (e);
}

/**
 * Allocation memory for node
 */
NodeAux * GuiGraph::createNode()
{
    int num = incNodeId();
    
	QString text =  QString( "Node %1").arg( num);// number here !!!!!!!
    GuiNode * node_p = new GuiNode (&text, this, num, &ss);

    node_p->setZValue( 1);
    addItem( node_p);

    return node_p;
}
/**
 * adds remove item to graph::removeEdge
 */
void GuiGraph::removeEdge (Edge* e)
{
	removeItem (addGui (e));
	Graph::removeEdge (e);
}

/**
 * adds remove item to graph::removeNode
 */
void GuiGraph::removeNode (Node* n)
{
	removeItem( addGui (n));
	Graph::removeNode (n);
}

/**
 * commit layout
 */
bool GuiGraph::applayLayout()
{
    GuiNode * node;
    GuiEdge * edge;

	for (NodeAux* iter = addAux( firstNode()); iter != 0;
		          iter = addAux( iter->nextNode()))
	{
		iter->commitPos (iter->x(), iter->y());
		if (!iter->real())
			iter->superscribe ( Qt::gray, "unreal");
	}

    for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
	{
        node->setMyAdjust ( node->real()? 3 : 1);
	}
	for ( edge = ( GuiEdge *)firstEdge(); isNotNullP( edge); edge = ( GuiEdge *) edge->nextEdge())
	{
        if ( edge != NULL)
		    edge->updatePosition();
	}
	return true;
}

/**
 * delete node
 */
void GuiGraph::deleteNode( int number)
{
	GuiNode *node;
	for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if ( node->userId() == number)
		{
			removeNode(node);
			break;
		}
}
