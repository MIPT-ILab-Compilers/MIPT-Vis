/**
 * File: GUI/GuiPoint.h - Implementational header for GuiPoint of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_POINT_H
#define GUI_POINT_H
#include <QtGui/QGraphicsRectItem>
#include "gui_impl.h"

class GuiEdge;
class GuiEdgePart;

/**
 * Class GuiPoint. 
 */
class GuiPoint:public QObject, public QGraphicsItem
{
public:
    enum {Type = QGraphicsItem::UserType + 3};
    GuiPoint( GuiEdge * edge, QGraphicsScene * scene);
    GuiPoint( GuiEdge * edge, QGraphicsItem * parent, QGraphicsScene * scene);
    inline QPolygonF polygon() const
    {
        return myPolygon;
    };
    inline void setMyColor( QColor color)
    {
        myColor = color;
    };
    inline void setMyAdjust( qreal adjust)
    {
        myAdjust = adjust;
    };
    inline void setFixed( bool fixed = true)
    {
        isFixed = fixed;
        setFlag( ItemIsMovable, !fixed);
        if( fixed)
        {
            myColor = Qt::green;
        }
        else
        {
            myColor = Qt::red;
        }
    }

    inline bool fixed()
    {
        return isFixed;
    }
    inline void setInit( bool init = true)
    {
        isInit = init;
    }

    inline bool init()
    {
        return isInit;
    }
    inline GuiEdgePart * end() const
    {
        return endPart;
    } 
    inline GuiEdgePart * start() const
    {
        return startPart;
    }

    inline void setEnd( GuiEdgePart * e)
    {
        endPart = e;
    }

    inline void setStart( GuiEdgePart * s)
    {
        startPart = s;
    }
    void addEdge( GuiEdge * line);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void updatePosition();
    int type() const
    {
        return Type;
    }
protected:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QVariant itemChange( GraphicsItemChange change, const QVariant &value);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
private:
    QList<GuiEdge *> edges;
    QPolygonF myPolygon;
    QColor myColor;
    qreal myAdjust;
    GuiEdge * edge; 
    GuiEdgePart * endPart;
    GuiEdgePart * startPart;
    bool isFixed;
    bool isInit;
};
#endif
