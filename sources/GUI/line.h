/**
 * File: GUI/line.h - Implementational header for edge of GUI in MiptVis tool.
 * Copyright (C) 2009  Ivan Mashintsev
 */
#ifndef LINE_H
#define LINE_H
/**
 * Class Edge. Node is movable. Double click create new node
 */
class Line : public QGraphicsLineItem
{
public:
    Line( Text *startItem, Text *endItem, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    inline void setColor(const QColor &color)
    { 
        myColor = color;
    }
    inline Text *startItem() const
    { 
        return myStartItem;
    }
    inline Text *endItem() const
    {
        return myEndItem;
    }
    inline void setEndItem( Text *endItem)
    {
        myEndItem = endItem;
    }
    inline void setStartItem( Text *startItem)
    {
        myEndItem = startItem;
    }
public slots:
    void updatePosition();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    Text *myStartItem;
    Text *myEndItem;
    QColor myColor;
    QPolygonF lineHead;
};
#endif
