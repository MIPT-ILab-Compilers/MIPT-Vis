/**
 * File: GUI/scene.h - Implementational header for Scene of GUI in MiptVis tool.
 * Copyright (C) 2009  Ivan Mashintsev
 */
#ifndef SCENE_H
#define SCENE_H
#include <QtGui/QGraphicsScene>

/**
 * Class Scene
 */
class Scene:public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject * parent=0);
protected:
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
};
#endif
