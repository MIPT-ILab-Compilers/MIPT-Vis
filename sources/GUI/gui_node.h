/**
 * File: GUI/GuiNode.h - Implementational header for GuiNode of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_NODE_H
#define GUI_NODE_H
#include <QtGui/QGraphicsTextItem>
#include <QtGui/QtGui>
#include <QtGui/QApplication>
#include <QtCore/QLineF>
#include "../Layout/layout_iface.h"
//#include "gui_impl.h"
class GuiEdge;
class GuiNode;
class GuiGraph;


class GuiEdge;
class GuiPoint;

/**
 * Class GuiNode.
 */
class GuiNode:public QGraphicsTextItem, public NodeAux
{
    Q_OBJECT

private:
    QPolygonF myPolygon;
    QColor myColor;
    qreal myAdjust;
    friend class GuiGraph;
    friend class GraphAux;

public:
    QString myText;
    enum {Type = QGraphicsItem::UserType + 1};
    GuiNode(  QString * text, GuiGraph * graph_p, int _id, 
        QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
	virtual ~GuiNode();
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

public slots:
    void setMyText(const QString & str);
};
#endif
