/**
 * File: GUI/scene.cpp - Implementation of Scene in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"


/**
 * Create rectengle with editable text after double click mouse
 */
void Scene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if( mouseEvent->button() != Qt::LeftButton)
        return;  
    if( !selectedItems().isEmpty())
    {
        if( selectedItems().first()->zValue() == -1000.0)
        {
            myMode = insertRect;
            Rectangle *rect = new Rectangle();
            Text *endItem = (qgraphicsitem_cast<Line *> (selectedItems().first()))->endItem();
            Text *startItem = rect;
            rect->setPos( mouseEvent->scenePos());
            rect->setZValue( 2);
            addItem( rect);
            (qgraphicsitem_cast<Line *> (selectedItems().first()))->setEndItem( rect);
            Line *line = new Line( rect, endItem);
            line->setColor( Qt::red);
            startItem->addLine( line);
            endItem->addLine( line);
            line->setZValue( -1000.0);
            addItem( line);
            line->updatePosition();
        }
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    }
    else
    {
        Text * node = new Text();
        node->setPos( mouseEvent->scenePos());
        node->setZValue(1);  
        addItem( node);
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    }
}

/**
 * Constructor for class Scene.
 */
Scene::Scene( QObject * parent):QGraphicsScene( parent)
{
}

void Scene::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if( mouseEvent->button() & Qt::RightButton)
    {
        myMode = insertLine;
        line = new QGraphicsLineItem( QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen( QPen( Qt::black));
        addItem( line);
    }
    QGraphicsScene::mousePressEvent( mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
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

void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if ( line != 0 && myMode == insertLine)
    {
        QList<QGraphicsItem *> startItems = items( line->line().p1());
        if ( startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items( line->line().p2());
        if ( endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        removeItem( line);
        delete line;
        if ( startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == Text::Type &&
            endItems.first()->type() == Text::Type &&
            startItems.first() != endItems.first())
        {
            Text *startItem = qgraphicsitem_cast<Text *> ( startItems.first());
            Text *endItem = qgraphicsitem_cast<Text *> ( endItems.first());
            Line *line = new Line( startItem, endItem);
            line->setColor( Qt::red);
            startItem->addLine( line);
            endItem->addLine( line);
            line->setZValue( -1000.0);
            addItem( line);
            line->updatePosition();
        }
    }
    line = 0;
    myMode = moveItem;
    QGraphicsScene::mouseReleaseEvent( mouseEvent);
}