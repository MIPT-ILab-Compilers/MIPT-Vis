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
    if( mouseEvent->button() != Qt::LeftButton)
    {
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
        return;  
    }
    if( selectedItems().isEmpty())
    {
        number++;
        GuiNode * node = newNode();
        node->setPos( mouseEvent->scenePos());
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    }
    QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( QObject * parent):myMode( insertRect), QGraphicsScene( parent)
{
    number = 0;
	line = NULL;
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( char * filename, QObject * parent):myMode( insertRect), QGraphicsScene( parent)
{
    number = 0;
	line = NULL;
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
    QGraphicsScene::mousePressEvent( mouseEvent);
}

/**
 * Move line when move mouse.
 */
void GuiGraph::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
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
            GuiEdge * lin = newEdge( startItem, endItem);
        }
    }
    line = NULL;
    myMode = moveItem;
    QGraphicsScene::mouseReleaseEvent( mouseEvent);
}

/**
 * Creation node in graph
 */
GuiNode * GuiGraph::newNode()
{
    GuiNode * node = ( GuiNode *) createNode();
    node->setZValue( 1);  
    addItem( node);
    return node;
}

/**
 * Allocation memory for edge
 */
void * GuiGraph::createEdge( GuiNode * pred, GuiNode * succ)
{
    return new GuiEdge( this, incEdgeId(), pred, succ);
}

/**
 * Allocation memory for node
 */
void * GuiGraph::createNode()
{
    int num = incNodeId();
    QString text =  QString( "Node %1").arg( num);
    return new GuiNode( &text, this, num);
}
/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
GuiEdge * GuiGraph::newEdge( GuiNode * pred, GuiNode * succ)
{
    GuiEdge * e =  ( GuiEdge *) createEdge( pred, succ);
    addItem( e);
    e->initPoint( pred, succ);
    pred->addEdge( e);
    succ->addEdge( e);
    e->setZValue( -1000.0);
    e->updatePosition();
    return e;
}
