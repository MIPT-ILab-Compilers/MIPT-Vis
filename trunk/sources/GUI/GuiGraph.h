/**
 * File: GUI/GuiGraph.h - Implementational header for GuiGraph of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_GRAPH_H
#define GUI_GRAPH_H
#include <QtGui/QGraphicsScene>
#include "gui_impl.h"

/**
 * Class GuiGraph
 */
class GuiGraph:public QGraphicsScene, public Graph
{
    Q_OBJECT
public:
    GuiGraph(QObject * parent=0);
    GuiGraph( char * filename, QObject * parent = 0);
    enum mode { insertLine, moveItem, insertRect};
    enum type { node, point, edge};
    void * createNode();
    void * createEdge( GuiNode * pred, GuiNode * succ);
    GuiNode * newNode();
    GuiEdge * newEdge( GuiNode * pred, GuiNode * succ);
protected:
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);
private:
    int number;
    mode myMode;
    QGraphicsLineItem * line;
};
#endif
