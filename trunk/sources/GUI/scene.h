/**
 * File: GUI/scene.h - Implementational header for Scene of GUI in MiptVis tool.
 * Copyright (C) 2009  Ivan Mashintsev
 */
#ifndef SCENE_H
#define SCENE_H
#include <QtGui/QGraphicsScene>
#include "gui_impl.h"

/**
 * Class Scene
 */
class Scene:public QGraphicsScene
{
    Q_OBJECT
            
public:
    Scene(QObject * parent=0);
    enum mode { insertLine, moveItem, insertRect};
protected:
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);
private:
    mode myMode;
    QGraphicsLineItem *line;
};
#endif
