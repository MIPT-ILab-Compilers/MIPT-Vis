/**
 * File: GUI/GuiNode.h - Implementational header for GuiNode of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_NODE_H
#define GUI_NODE_H
#include <QtGui/QGraphicsTextItem>
#include "gui_impl.h"

class GuiEdge;
class GuiPoint;

/**
 * Class GuiNode.
 */
class GuiNode:public QGraphicsTextItem, public NodeAux
{
    Q_OBJECT

private:
    QList<GuiEdge *> edges;
    QPolygonF myPolygon;
    QColor myColor;
    qreal myAdjust;
    friend class GuiGraph;
    friend class GraphAux;

public:
    enum {Type = QGraphicsItem::UserType + 1};
    GuiNode(  QString * text, GuiGraph * graph_p, int _id, 
        QGraphicsItem * parent = 0, QGraphicsScene * scene = 0):
        myAdjust(0),
        QGraphicsTextItem( parent, scene),
        NodeAux( ( GraphAux *) ( graph_p), _id)
{
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    setPlainText( *text);
    setTextWidth ( 100); //Set width of node
    setMyAdjust( 3);
    setMyColor( Qt::green);
    setFlag( QGraphicsItem::ItemIsMovable, true); // Set node can move
    setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
    setTextInteractionFlags( Qt::NoTextInteraction);
    myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
                  << (boundingRect().topRight()) << (boundingRect().topLeft())
                  << (boundingRect().bottomLeft());
}
    inline QPolygonF polygon() const
    {
        return myPolygon;
    };
    inline void setMyColor( QColor color)
    {
        myColor = color;
    };
    inline QColor getMyColor()
    {
        return myColor;
    };
    inline void setMyAdjust( qreal adjust)
    {
        myAdjust = adjust;
    };
    void addEdge( GuiEdge * line);
    QRectF boundingRect() const;

    int type() const
    {
        return Type;
    }
protected:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QVariant itemChange( GraphicsItemChange change, const QVariant &value);
    void focusInEvent ( QFocusEvent * event);
    void focusOutEvent ( QFocusEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void commitPos( int x,int y);
    void superscribe (QColor color,QString text);
signals:
    void isClicked(const QString & str);
};
#endif
