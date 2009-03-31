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
    if( !selectedItems().isEmpty()) return;
    Rectangle * node = new Rectangle();
    addItem( node); // Add rectangle on the scene 
    node->setPos( mouseEvent->scenePos()); // Set position of rectangle on the scene 
    Text * text = new Text();
    addItem( text); // Add text on the scene
    text->setParentItem( node); // Text is associated with node
    text->setPos( -25, -25);
}

/**
 * Constructor for class Scene.
 */
Scene::Scene( QObject * parent):QGraphicsScene( parent)
{
}

