/**
 * File: GUI/GuiEdgePart.h - Implementational header for GuiEdgePart of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_EDGE_PART_H
#define GUI_EDGE_PART_H

#include "gui_impl.h"

class GuiEdgePart: public QGraphicsItem
{
    GuiEdge * edge;
    QPointF startP;
    QPointF endP;
    GuiPoint * startPoint;
    GuiPoint * endPoint;
    QPointF topLeft;
    QPointF btmRight;
public:
    enum { Type = QGraphicsItem::UserType + 2};
    //inline GuiPoint * start() const
    //{
    //    return startPoint;
    //}

    //inline GuiPoint * end() const
    //{
    //    return endPoint;
    //}

    //inline void setStart( GuiPoint * start)
    //{
    //    startPoint = start;
    //    if ( start != NULL)
    //    {
    //        start->setEnd( this);
    //    }  
    //}

    //inline void setEnd( GuiPoint * end)
    //{
    //    endPoint = end;
    //    if ( end != NULL)
    //    {
    //        end->setStart( this);
    //    } 
    //}

    void GuiEdgePart::updatePosition();
    GuiEdgePart( GuiEdge * e, GuiPoint * start, GuiPoint * end, QGraphicsScene * scene);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    GuiPoint * addPoint( QPointF p);
    int type() const
    {
        return Type;
    }
protected:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent( QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event);
};
#endif