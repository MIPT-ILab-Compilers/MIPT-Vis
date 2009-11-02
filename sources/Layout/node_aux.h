#ifndef NODE_AUX_H
#define NODE_AUX_H

#include<QtGui/QColor>
#include<QtCore/QString>

/**
 * Representation of various auxiliary node attributes
 */
class NodePropertiesAux
{
protected:
    int rang_priv; // number of node's layer
    int pos_priv;  // absolute position in layer

	int x_priv;
	int y_priv;
	int width_priv;
	int height_priv;
	bool is_real; //indicates that the node must be showed
public:
    // Default constructor
    NodePropertiesAux();
    // Gets for auxiliary properties
    inline int rang() const
    {
        return rang_priv;
    }
    inline int posAux() const
    {
        return pos_priv;
    }
	inline int x() const
    {
        return x_priv;
    }
	inline int y() const
    { 
        return y_priv;
    }
	inline int width() const 
    {
        return width_priv;
    }
	inline int height() const 
    { 
        return height_priv;
    }
	inline bool real() const
	{
		return is_real;
	}
    // Sets for auxiliary properties
    inline void setRang( int rang)
    {
        rang_priv = rang;
    }
    inline void setPosAux( int posAux)
    {
        pos_priv = posAux;
    }
	inline void setX( int x) 
    {
        x_priv = x;
    }
	inline void setY( int y)
    { 
        y_priv = y;
    }
	inline void setWidth( int width)
    { 
        width_priv = width;
    }
	inline void setHeight( int height)
    {
        height_priv = height;
    }
	inline void setReal (bool nreal)
	{
		is_real = nreal;
	}
};

/**
 * Auxiliary node representation class. 
 */
class NodeAux: public Node, public NodePropertiesAux
{
protected:
    NodeAux( GraphAux *graph_p, int _id);
    friend class GraphAux;

	
	virtual void writeByXMLWriter( xmlTextWriterPtr writer);
	virtual void readByXML (xmlNode * cur_node);
public:
     /**
     * Get node's corresponding auxiliary graph
     */
    inline GraphAux * getGraph() const;

    /**
       * Add an edge to this node in specified direction
     */
    void addEdgeInDir( EdgeAux *edge, GraphDir dir)
    {
        Node::addEdgeInDir( static_cast< Edge*> (edge), dir);
    }

    /**
     * Add predecessor edge
     */
    inline void addPred( EdgeAux *edge)
    {
        addEdgeInDir( edge, GRAPH_DIR_UP);
    }

    /**
     * Add successor edge
     */
    inline void addSucc( EdgeAux *edge) 
    {
        addEdgeInDir( edge, GRAPH_DIR_DOWN);
    }

     /**
     *  Iteration through edges routines.
     *
     *  Set iterator to beginning of edge list and return first edge
     */
    inline EdgeAux* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< EdgeAux*>( Node::firstEdgeInDir( dir));
    }
    /**
     * Advance iterator and return next edge in specified direction
     * NOTE: If end of list is reached we return NULL for first time and fail if called once again
     */
    inline EdgeAux* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< EdgeAux*>( Node::nextEdgeInDir( dir));
    }

    /** 
     * Corresponding iterators for successors/predeccessors.
     * NOTE: See firstEdgeInDir and other ...InDir routines for details
     */
    inline EdgeAux* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline EdgeAux* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline EdgeAux* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    inline EdgeAux* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    }
    virtual void debugPrint();
    virtual void commitPos(int x,int y)
    {
    }
    virtual void superscribe(QColor color, QString text)
    {
    }
};

#endif