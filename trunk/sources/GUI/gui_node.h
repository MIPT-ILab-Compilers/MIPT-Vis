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
#include "StyleSheet.h"
class GuiEdge;
class GuiNode;
class GuiGraph;
class GuiEdge;
class GuiPoint;

extern bool VirtualNodesDrawing;

/**
 * Representation of various node attributes
 */
class NodeProperties
{
    char * color_priv;
	char * label_priv;
	char * shape_priv;
	char * text_priv;

	StId style_priv;
	StyleSheet *ss_priv;

public:
	NodeProperties (StyleSheet* ss);
    /** Data reading */
    inline char * color() const 
    {
        return color_priv;
    }
	inline char * label() const
    { 
        return label_priv;
    }
	inline char * shape() const
    { 
        return shape_priv;
    }
	inline char * textPriv() const
    { 
        return text_priv;
    }
	inline StId style () const
    { 
        return style_priv;
    }
	inline void applStyle (QPainter * painter)// const
    { 
        ss_priv->applayStyle (style_priv, painter);
    }
    /** Data writing */ 
    inline void setColor( char * color)
    {
        color_priv = color;
    }
	inline void setLabel( char * label) 
    {
        label_priv = label;
    }
	inline void setShape( char * shape) 
    { 
        shape_priv = shape;
    }
	inline void setTextPriv( char * textPriv) 
    { 
        text_priv = textPriv;
    }
	inline void setStyle (char * stName) 
    { 
        style_priv = ss_priv->getId (stName);
    }
};

/**
 * Class GuiNode.
 */
class GuiNode:public QGraphicsTextItem, public NodeAux, public NodeProperties
{
    Q_OBJECT

private:
    QPolygonF myPolygon;
    QColor myColor;
    qreal myAdjust;
    friend class GuiGraph;
    friend class GraphAux;

protected:
	virtual void writeByXMLWriter( xmlTextWriterPtr writer);
	virtual void readByXML ( xmlNode * cur_node);
	virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent *event);

public:
    QString myText;
    enum { Type = QGraphicsItem::UserType + 1};
    GuiNode( QString * text, GuiGraph * graph_p, int _id, StyleSheet *ss,
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
	inline QString getMyText()
    {
        return myText;
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
    void commitPos( int x, int y);
    void superscribe ( QColor color, QString text);

public slots:
    void setMyText( const QString & str);
	void textChange();
	void emitDelete();

signals:
	void deleteMe( int number);
};
#endif
