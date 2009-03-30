/**
 * File: GUI/text_item.h - Implementational header for text item of GUI in MiptVis tool.
 * Copyright (C) 2009  Ivan Mashintsev
 */
#ifndef TEXTITEM_H
#define TEXTITEM_H
#include <QtGui/QGraphicsTextItem>

/**
 * Class Text. Text item is assosiated with node and is able to edit
 */
class Text:public QGraphicsTextItem
{
public:
    Text( QGraphicsItem * parent = 0);
protected:
    void focusInEvent( QFocusEvent * event);
    void focusOutEvent ( QFocusEvent * event );
};
#endif
