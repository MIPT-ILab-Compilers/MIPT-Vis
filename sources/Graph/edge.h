/**
 * File: edge.h - Edge class definition, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_H
#define EDGE_H

#include <vector>

/**
 * Representation of a point on 2d surface.
 */
typedef struct
{
	int x;
    int y;
} EdgePoint;

/**
 * Implements collection of edge properties and provides accessors for them
 */
class EdgeProperties
{
    int user_id;
    int prob_priv;
	int thickness_priv;
	char * color_priv;
	char * style_priv;
	char * label_priv;
	vector<EdgePoint *> points;
public:
    EdgeProperties()
    {
        /** Default values */
        prob_priv = 1;
	    thickness_priv = 1;
	    color_priv = NULL;
	    style_priv = NULL;
	    label_priv = NULL;
    }
    inline int userId() const
    {
        return user_id;
    }
    inline int prob() const
    {
        return prob_priv;
    }
    inline int thickness() const
    {
        return thickness_priv;
    }
	inline char * color() const
    {
        return color_priv;
    }
	inline char * style() const
	{
        return style_priv;
    }
    inline char * label() const
    {
        return label_priv;
    }
    inline EdgePoint * point( int n) const
    {
        return points[ n];
    }
    inline int pointsNum() const
    {
		return ((int)points.size() - 1);
    }
    /** Data writing routines */
    inline void setUserId( int i)
    {
        user_id = i;
    }
    inline void setProb (int prob)
    {
        prob_priv = prob;
    }
	inline void setThickness( int thickness)
    {
        thickness_priv = thickness;
    }
	inline void setColor( char * color)
    {
        color_priv = color;
    }
	inline void setStyle( char * style)
    {
        style_priv = style;
    }
	inline void setLabel( char * label)
    {
        label_priv = label;
    }
    inline void setPoint( EdgePoint *p, int n)
    {
        points[ n] = p;
    }
    inline void addPoint( EdgePoint *p)
    {
        points.push_back( p);
    }
    inline void initPoints( int n)
    {
        points.resize( n, NULL);
    }
};
/**
 *  Edge class implements basic concept of graph edge.
 *  It has two Nodes as its end points. As edge is member of 3 lists it
 *  has 3 corresponding list items pointing to it in these lists
 */
class Edge: public Marked, public Numbered, public EdgeProperties
{

    /** Graph part */
    int unique_id; //Unique ID

    Graph * graph; //Graph
    EdgeListItem graph_it; //Position in Graph's list of edges

	virtual void readEdgePointsFromXMLDoc( xmlNode * a_node);
	virtual void writeByXMLWriter( xmlTextWriterPtr writer);
	virtual void writePointsByXMLWriter( xmlTextWriterPtr writer);
	
    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    EdgeListItem n_it[ GRAPH_DIRS_NUM];//Position in each node's list

    /** Graph and Node have access to Edge's members */
    friend class Graph;
    friend class Node;
protected:
    /** Constructors are made protected, only nodes and graph can create edges */
    Edge( Graph *graph_p, int _id, Node *_pred, Node* _succ):
        unique_id( _id), graph( graph_p), graph_it()
    {
        graph_it.setData( (Edge*) this);
        n_it[ GRAPH_DIR_UP] = EdgeListItem();
        n_it[ GRAPH_DIR_DOWN] = EdgeListItem();
        n_it[ GRAPH_DIR_UP].setData( (Edge*)this);
        n_it[ GRAPH_DIR_DOWN].setData( (Edge*)this);
        setPred( _pred);
        setSucc( _succ);
    }
public:
    /**
     * get edge's unique ID
     */
    inline int id() const
    {
        return unique_id;
    }

    /**
     * get edge's corresponding graph
     */
    virtual inline Graph * getGraph() const
    {
        return graph;
    }

    /** Pivate routines dealing with iterators */
    void setGraphIt( EdgeListItem g_it)
    {
        graph_it = g_it;
    }
    /** 
     * Return iterator pointing to this edge in graph's edge list
     */
    virtual EdgeListItem *getGraphIt()
    {
        return &graph_it;
    }

    /** 
     * set iterator pointing to this edge in graph's edge list
     */
    void setListIt( GraphDir dir, EdgeListItem it)
    {
        n_it[ dir] = it;
    }

    /** 
     * Return iterator pointing to this edge in node's edge
     * list in corresponding direction
     */
    virtual EdgeListItem *getNodeIt( GraphDir dir)
    {
        return &n_it[ dir];
    }

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    void detachFromNode( GraphDir dir);
    /** 
     *  Destructor. delete edge from list in graph.
     *  Deletion from node lists MUST be performed manually.
     *  Example: 
     *      Graph graph;
     *      Edge * edge = graph.newEdge();
     *  
     *      //Typical deletion of edge is done by consequent calls of
     *      edge->detachFromNode( GRAPH_DIR_UP);
     *      edge->detachFromNode( GRAPH_DIR_DOWN);
     *      delete edge;
     */
    virtual ~Edge();

    /**
     * Connect edge to a node in specified direction.
     * Note that node treats this edge in opposite direction. I.e. an edge that has node in
     * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
     */
    virtual void setNode( Node *n, GraphDir dir)
    {
        graphassert( isNotNullP( n));
        nodes[ dir] = n;
        if ( n != NULL)
        {
            n->addEdgeInDir( (Edge *)this, 
                ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
        }
    }
    
    /**
     * Connect edge with given node as a predecessor
     */
    virtual inline void setPred( Node *n)
    {
        setNode( n, GRAPH_DIR_UP);
    }
    /**
     * Connect edge with given node as a successor
     */
    virtual inline void setSucc( Node *n)
    {
        setNode( n, GRAPH_DIR_DOWN);
    }

    /**
     * get node in specified direction
     */
    virtual inline Node *getNode( GraphDir dir) const 
    {
        return nodes[ dir];
    }
    /**
     * get predecessor of edge
     */
    virtual inline Node *pred() const 
    {
        return getNode( GRAPH_DIR_UP);
    }
    /**
     * get successor of edge
     */
    virtual inline Node *succ() const 
    {
        return getNode( GRAPH_DIR_DOWN);
    }
    /**
     * Print edge in dot fomat to stdout
     */
    virtual void debugPrint();
};

#endif
