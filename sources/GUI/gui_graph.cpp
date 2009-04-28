/**
 * File: GUI/GuiGraph.cpp - Implementation of GuiGraph in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

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
        node->setLabel( "NODE");
        node->setShape( "rectangle");
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    }
    QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( QObject * parent):myMode( insertRect), Graph(), QGraphicsScene( parent)
{
    number = 0;
	line = NULL;
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( char * filename, QObject * parent):myMode( insertRect), Graph(), QGraphicsScene( parent)
{
	GuiNode * node;
	GuiEdge * edge;
    number = 0;
	line = NULL;

    readFromXML( filename);

	for ( node = ( GuiNode *)firstNode(); !endOfNodes(); node = ( GuiNode *)nextNode())
	{
		node->setPos( node->Node::x(), node->Node::y());
	}
	for ( edge = ( GuiEdge *)firstEdge(); !endOfEdges(); edge = ( GuiEdge *) nextEdge())
	{
		edge->updatePosition();
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
}


/**
 * Allocation memory for edge
 */
GuiEdge * GuiGraph::createEdge( Node * pred, Node * succ)
{
	GuiEdge * e = new GuiEdge( this, incEdgeId(), ( GuiNode *)pred, ( GuiNode *)succ);
	addItem( e);
    e->initPoint( ( GuiNode *)pred, ( GuiNode *)succ);
    e->initPoints( 1);
    e->setStyle( "Solid");
    ( ( GuiNode *)pred)->addEdge( e);
    ( ( GuiNode *)succ)->addEdge( e);
    e->setZValue( -1000.0);
    e->updatePosition();

    return e;
}

/**
 * Allocation memory for node
 */
GuiNode * GuiGraph::createNode()
{
    int num = incNodeId();
	QString text =  QString( "Node %1").arg( num);
	GuiNode * node_p = new GuiNode( &text, this, num);

    node_p->setZValue( 1);
    addItem( node_p);

    return node_p;
}

void GuiGraph::initPos()
{
    for( Node * nd = firstNode(); nd != NULL; nd = nextNode())
    {
        GuiNode * gn = ( GuiNode *)newNode();
        gn->setPos( (qreal)nd->x(), (qreal)nd->y());
        
    }
}