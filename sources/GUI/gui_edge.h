/**
 * File: GUI/GuiEdge.h - Implementational header for GuiEdge of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_EDGE_H
#define GUI_EDGE_H
/**
 * Class GuiEdge. GuiEdge is movable. Double click create new point on the GuiEdge
 */
class GuiEdge:public QGraphicsLineItem, public Edge
{
public:
    inline GuiPoint * startItem() const
    { 
        return startPoint;
    }
    inline GuiPoint * endItem() const
    {
        return endPoint;
    }
    inline void setEndItem( GuiPoint * endItem)
    {
        endPoint = endItem;
    }
    inline void setStartItem( GuiPoint * startItem)
    {
        startPoint = startItem;
    }
    void addPoint( GuiPoint * point);
    void addEdgePart( GuiEdgePart * part);
    void initPoint( GuiNode * startItem, GuiNode * endItem);
    void showPoints();
    void hidePoints();
    void updatePosition();

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event);
private:
    GuiEdge( GuiGraph * graph_p, int _id, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
    GuiPoint * startPoint;
    GuiPoint * endPoint;
    QList< GuiPoint *> points;
    QList< GuiEdgePart *> edgePart;
    friend class GuiGraph;
    friend class GuiNode;
    friend class Graph;
    friend class Node;
};
#endif
