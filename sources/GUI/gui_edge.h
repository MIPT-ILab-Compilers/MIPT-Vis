/**
 * File: GUI/GuiEdge.h - Implementational header for GuiEdge of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_EDGE_H
#define GUI_EDGE_H

/**
 * Implements collection of edge properties and provides accessors for them
 */
class EdgeProperties
{
    int prob_priv;
	int thickness_priv;
	char * edge_color_priv;
	char * edge_label_priv;
	
	StId edge_style_priv;
	StyleSheet *edge_style_sheet_priv;
public:
    EdgeProperties (StyleSheet* edge_style_sheet_priv)
		:edge_style_sheet_priv (edge_style_sheet_priv)
    {
        /** Default values */
        prob_priv = 1;
	    thickness_priv = 1;
	    edge_color_priv = NULL;
	    edge_style_priv = NULL;
	    edge_label_priv = NULL;
    }
    inline int prob() const
    {
        return prob_priv;
    }
    inline int thickness() const
    {
        return thickness_priv;
    }
	inline char * edgeColor() const
    {
        return edge_color_priv;
    }
    inline char * edgeLabel() const
    {
        return edge_label_priv;
    }
	inline const QString& edgeStName() const
    { 
        return edge_style_sheet_priv->getStName (edge_style_priv);
    }
	inline void edgeApplStyle (QPainter * painter, const QStyleOptionGraphicsItem * option)
    { 
        edge_style_sheet_priv->applayStyle (edge_style_priv, painter, option);
    }
    /** Data writing routines */
    inline void setProb( int prob)
    {
        prob_priv = prob;
    }
	inline void setThickness( int thickness)
    {
        thickness_priv = thickness;
    }
	inline void setEdgeColor( char * color)
    {
        edge_color_priv = color;
    }
	inline void setEdgeLabel( char * label)
    {
        edge_label_priv = label;
    }
	inline void setEdgeStyle (const QString& stName) 
    { 
        edge_style_priv = edge_style_sheet_priv->getId (stName);
    }
};

/**
 * Class GuiEdge. GuiEdge is movable. Double click create new point on the GuiEdge
 */
class GuiEdge:public QGraphicsItem, public EdgeAux, public EdgeProperties
{
    QPointF edge_start_point_priv;
	QPointF edge_start_dir_priv;
    QPointF edge_end_point_priv;
	QPointF edge_end_dir_priv;

	QPainterPath edge_curve_priv;
	bool edge_valid_priv;

    QPointF edge_top_left_priv;
    QPointF edge_bottom_right_priv;

public:
    enum { Type = QGraphicsItem::UserType + 2};//!!!I do not know what type here needed
public:
    void updatePosition();
    QRectF boundingRect() const;
    QPainterPath shape() const;
	GuiNode* insertNode ( QPointF p);
    int type() const
    {
        return Type;
    }

protected:
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent( QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event);
	
	virtual void writeByXmlWriter( xmlTextWriterPtr writer);
	virtual void readByXml ( xmlNode * cur_node);
private:
    GuiEdge( GuiGraph * graph_p, int _id, StyleSheet* ss, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
	virtual ~GuiEdge();
    friend class GuiGraph;
    friend class GuiNode;
    friend class GraphAux;
    friend class NodeAux;
};
#endif
