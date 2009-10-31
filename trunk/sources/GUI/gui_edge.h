/**
 * File: GUI/GuiEdge.h - Implementational header for GuiEdge of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_EDGE_H
#define GUI_EDGE_H
/**
 * Class GuiEdge. GuiEdge is movable. Double click create new point on the GuiEdge
 */
class GuiEdge:public QGraphicsItem, public EdgeAux
{
    QPointF startP;
    QPointF endP;
    QPointF topLeft;
    QPointF btmRight;

public:
    enum { Type = QGraphicsItem::UserType + 2};//!!!I do not know what type here needed
public:
    void updatePosition();
    QRectF boundingRect() const;
    QPainterPath shape() const;
	GuiNode* insertNode (QPointF p);
    int type() const
    {
        return Type;
    }

protected:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent( QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event);
private:
    GuiEdge( GuiGraph * graph_p, int _id, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
	virtual ~GuiEdge();
    friend class GuiGraph;
    friend class GuiNode;
    friend class GraphAux;
    friend class NodeAux;
};
#endif
