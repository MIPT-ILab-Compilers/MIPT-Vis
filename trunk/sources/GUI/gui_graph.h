/**
 * File: GUI/GuiGraph.h - Implementational header for GuiGraph of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_GRAPH_H
#define GUI_GRAPH_H
#include <QtGui/QGraphicsScene>
#include "../Layout/layout_iface.h"
#include "StyleSheet.h"
class GuiEdge;
class GuiNode;

/**
 * Class GuiGraph
 */
class GuiGraph:public QGraphicsScene, public GraphAux
{
    Q_OBJECT
public:
	
    GuiGraph( QObject * parent=0);
    GuiGraph( char * filename, QObject * parent = 0);
    enum mode { insertLine, moveItem, insertRect};
    enum type { node, point, edge};
    
	virtual void readAttribsFromXml (xmlNode * a_node);

    NodeAux * createNode();
    
    EdgeAux * createEdge( Node * pred, Node * succ); //!!! may be need another return type

    inline QGraphicsItem * getNodeItem()
    {
        GuiNode * gui_node_p = ( GuiNode *)firstNode();
        return (QGraphicsItem *) gui_node_p;       
    }

	virtual void removeEdge (Edge* e);
	virtual void removeNode (Node* n);

protected:
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);
    bool applayLayout();

private:
    int number;
    mode myMode;
    QGraphicsLineItem * line;
    xmlDoc * xml_doc;
	StyleSheet ss;

signals:
    void isClicked();

public slots:
	void deleteNode( int number);
};
#endif
